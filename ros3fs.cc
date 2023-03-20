// ozonefs: FUSE filesystem for Ozone
// Copyright (C) 2023 Akira Kawata
// This program can be distributed under the terms of the GNU GPLv2.

#include <cstddef>
#include <cstdint>
#include <memory>
#include <set>
#include <sys/stat.h>
#define FUSE_USE_VERSION 31

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <glog/logging.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/HeadObjectRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>

#define LOG_KEY_VALUE(key, value) " " << key << "=" << value
#define LOG_KEY(key) LOG_KEY_VALUE(#key, key)

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
namespace {
struct options {
  int show_help;
  const char *endpoint;
  const char *bucket_name;
  const char *cache_dir;
} options;

#define OPTION(t, p)                                                           \
  { t, offsetof(struct options, p), 1 }
const struct fuse_opt option_spec[] = {OPTION("-h", show_help),
                                       OPTION("--help", show_help),
                                       OPTION("--endpoint=%s", endpoint),
                                       OPTION("--bucket_name=%s", bucket_name),
                                       OPTION("--cache_dir=%s", cache_dir),
                                       FUSE_OPT_END};

void *OzoneFSInit(struct fuse_conn_info *conn, struct fuse_config *cfg) {
  (void)conn;
  cfg->kernel_cache = 1;
  return NULL;
}

enum Type { kFile, kDirectory };

struct MetaData {
  std::filesystem::path path;
  uint64_t size;
  Type type;
};

std::string SerializeMetaData(const std::vector<MetaData> &meta_datas) {
  nlohmann::json j;
  for (const auto &md : meta_datas) {
    nlohmann::json j2;
    j2["path"] = md.path;
    j2["size"] = md.size;
    j2["type"] = md.type;
    j.push_back(j2);
  }
  return j.dump();
}

std::vector<MetaData> DeserializeMetaData(const std::string &json) {
  std::vector<MetaData> meta_datas;
  nlohmann::json j = nlohmann::json::parse(json);
  for (const auto &j2 : j) {
    MetaData md;
    md.path = std::filesystem::path(j2["path"]);
    md.size = j2["size"];
    md.type = j2["type"];
    meta_datas.push_back(md);
  }
  return meta_datas;
}

class OzoneFSContext {
public:
  OzoneFSContext(OzoneFSContext const &) = delete;
  void operator=(OzoneFSContext const &) = delete;
  static OzoneFSContext &GetContext() { return GetContextImpl("", "", ""); }
  static void InitContext(const std::string &endpoint,
                          const std::string &bucket_name,
                          const std::filesystem::path &cache_dir) {
    GetContextImpl(endpoint, bucket_name, cache_dir);
  }

  // Full directory path -> (Full directory or file path -> MetaData)
  std::map<std::filesystem::path, std::map<std::filesystem::path, MetaData>>
  GetMetaData() {
    if (!MetaDataCache) {
      const auto c = FetchMetaData();
      std::map<std::filesystem::path, std::map<std::filesystem::path, MetaData>>
          md;
      for (const auto &m : c) {
        md[m.path.parent_path()][m.path] = m;
      }
      MetaDataCache = md;
    }
    return *MetaDataCache;
  }

  void CopyFile(const std::string &src, const std::string &dst) {
    {
      using namespace Aws;
      SDKOptions options;
      options.loggingOptions.logLevel = Utils::Logging::LogLevel::Debug;

      // The AWS SDK for C++ must be initialized by calling Aws::InitAPI.
      InitAPI(options);
      {
        Aws::Client::ClientConfiguration config;
        config.endpointOverride = endpoint_;
        S3::S3Client client(config);

        Aws::S3::Model::GetObjectRequest request;
        request.SetBucket(bucket_name_);
        request.SetKey(src);

        Aws::S3::Model::GetObjectOutcome outcome = client.GetObject(request);

        if (!outcome.IsSuccess()) {
          const Aws::S3::S3Error &err = outcome.GetError();
          LOG(FATAL) << "Error: GetObject: " << err.GetExceptionName() << ": "
                     << err.GetMessage() << std::endl;
        } else {
          LOG(INFO) << "Successfully retrieved '" << src << "' from '"
                    << "bucket1/"
                    << "'." << std::endl;

          std::ofstream ofs(dst);
          ofs << outcome.GetResult().GetBody().rdbuf();
        }
      }

      // Before the application terminates, the SDK must be shut down.
      ShutdownAPI(options);
    }
  }

private:
  const std::string endpoint_;
  const std::string bucket_name_;
  const std::filesystem::path cache_dir_;
  const std::filesystem::path meta_data_path_;
  std::optional<std::map<std::filesystem::path,
                         std::map<std::filesystem::path, MetaData>>>
      MetaDataCache = std::nullopt;

  OzoneFSContext(const std::string &endpoint, const std::string &bucket_name,
                 const std::filesystem::path &cache_dir)
      : endpoint_(endpoint), bucket_name_(bucket_name),
        cache_dir_(std::filesystem::canonical(cache_dir)),
        meta_data_path_(std::filesystem::canonical(cache_dir) /
                        "meta_data.json") {
    CHECK_NE(endpoint, "");
    CHECK_NE(bucket_name, "");
    CHECK(std::filesystem::exists(cache_dir));
    LOG(INFO) << "OzoneFSContext initialized with endpoint=" << endpoint
              << " bucket_name=" << bucket_name
              << " cache_dir=" << std::filesystem::canonical(cache_dir);
  }

  static OzoneFSContext &
  GetContextImpl(const std::string &endpoint, const std::string &bucket_name,
                 const std::filesystem::path &cache_dir) {
    static OzoneFSContext context(endpoint, bucket_name, cache_dir);
    return context;
  }

  std::vector<MetaData> FetchMetaData() {
    if (std::filesystem::exists(meta_data_path_)) {
      LOG(INFO) << "Loading meta data from " << meta_data_path_;
      std::ifstream ifs(meta_data_path_);
      std::string json((std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));
      return DeserializeMetaData(json);
    }

    std::set<std::filesystem::path> file_and_dirs;
    std::vector<MetaData> result_files;

    {
      Aws::SDKOptions options;
      Aws::InitAPI(options);

      {
        Aws::Client::ClientConfiguration clientConfig;
        clientConfig.endpointOverride = endpoint_;
        Aws::S3::S3Client s3Client(clientConfig);

        Aws::S3::Model::ListObjectsRequest objectsRequest;
        objectsRequest.SetBucket(bucket_name_);

        Aws::S3::Model::ListObjectsOutcome objectsOutcome;
        bool isTruncated = false;
        std::string nextMarker;

        do {
          if (nextMarker != "") {
            objectsRequest.SetMarker(nextMarker);
          }
          objectsOutcome = s3Client.ListObjects(objectsRequest);
          if (objectsOutcome.IsSuccess()) {
            LOG(INFO) << "Objects in bucket: "
                      << objectsOutcome.GetResult().GetContents().size()
                      << std::endl;

            Aws::Vector<Aws::S3::Model::Object> objects =
                objectsOutcome.GetResult().GetContents();

            for (const auto &object : objects) {
              result_files.push_back(
                  MetaData{.path = "/" + object.GetKey(),
                           .size = static_cast<uint64_t>(object.GetSize()),
                           .type = kFile});
            }

            isTruncated = objectsOutcome.GetResult().GetIsTruncated();
            nextMarker = objectsOutcome.GetResult().GetNextMarker();
            LOG(INFO) << "Next marker: " << nextMarker << std::endl;
          } else {
            LOG(WARNING) << "Error listing objects in bucket: "
                         << objectsOutcome.GetError().GetMessage() << std::endl;
            isTruncated = false;
          }
        } while (isTruncated);

        LOG(INFO) << "Done listing objects in bucket" << std::endl;
      }
      Aws::ShutdownAPI(options);
    }

    std::set<std::filesystem::path> dirs;
    std::vector<MetaData> result_dirs;

    for (const auto &f : result_files) {
      std::filesystem::path p = f.path;
      while (true) {
        p = p.parent_path();
        if (!dirs.insert(p).second) {
          break;
        }
        CHECK(file_and_dirs.insert(p).second);
        result_dirs.push_back(
            MetaData{.path = p, .size = 0, .type = kDirectory});
      }
    }

    result_files.insert(result_files.end(),
                        std::make_move_iterator(result_dirs.begin()),
                        std::make_move_iterator(result_dirs.end()));

    LOG(INFO) << "Save metadata to " << meta_data_path_;
    std::ofstream ofs(meta_data_path_);
    ofs << SerializeMetaData(result_files);

    return result_files;
  }
};

int OzoneFSGetattr(const char *path_c_str, struct stat *stbuf,
                   struct fuse_file_info *fi) {
  LOG(INFO) << "OzoneFSGetattr" << LOG_KEY(path_c_str);

  (void)fi;

  memset(stbuf, 0, sizeof(struct stat));

  // Check https://linuxjm.osdn.jp/html/LDP_man-pages/man2/stat.2.html for
  // S_IFDIR and S_IFREG.

  const std::filesystem::path path(path_c_str);
  if (path == "/") {
    stbuf->st_mode = S_IFDIR | 0444;
    stbuf->st_nlink = 2;
    LOG(INFO) << "OzoneFSGetattr: " << LOG_KEY(path) << " is the root.";
    return 0;
  }

  const auto all_files = OzoneFSContext::GetContext().GetMetaData();
  if (all_files.contains(path.parent_path()) &&
      all_files.at(path.parent_path()).contains(path)) {
    const auto &f = all_files.at(path.parent_path()).at(path);
    if (f.type == kDirectory) {
      stbuf->st_mode = S_IFDIR | 0444;
      stbuf->st_nlink = 2;
      LOG(INFO) << "OzoneFSGetattr: " << LOG_KEY(path) << " is a directory.";

    } else {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      LOG(INFO) << "OzoneFSGetattr: " << LOG_KEY(path) << " is a normal file.";
      stbuf->st_size = f.size;
    }
    return 0;
  }

  LOG(INFO) << "OzoneFSGetattr: " << LOG_KEY(path) << " does not exist.";
  return -ENOENT;
}

int OzoneFSReaddir(const char *path_c_str, void *buf, fuse_fill_dir_t filler,
                   off_t offset, struct fuse_file_info *fi,
                   enum fuse_readdir_flags flags) {
  (void)offset;
  (void)fi;
  (void)flags;

  const std::filesystem::path path(path_c_str);
  LOG(INFO) << "OzoneFSReaddir" << LOG_KEY(path);

  filler(buf, ".", NULL, 0, static_cast<fuse_fill_dir_flags>(0));
  filler(buf, "..", NULL, 0, static_cast<fuse_fill_dir_flags>(0));

  const auto &metadata = OzoneFSContext::GetContext().GetMetaData();
  if (metadata.contains(path)) {
    const auto &files = metadata.at(path);
    for (const auto &file : files) {
      if (file.second.path != "/") {
        // TODO: Refactor this.
        std::string filler_str =
            path == "/"
                ? file.second.path.string().substr(path.string().size())
                : file.second.path.string().substr(path.string().size() + 1);
        LOG(INFO) << "OzoneFSReaddir: Found " << LOG_KEY(file.second.path)
                  << " in " << LOG_KEY(path) << LOG_KEY(filler_str);
        filler(buf, filler_str.c_str(), NULL, 0,
               static_cast<fuse_fill_dir_flags>(0));
      }
    }
  }

  return 0;
}

int OzoneFSOpen(const char *path, struct fuse_file_info *fi) {
  LOG(INFO) << "OzoneFSOpen" << LOG_KEY(path);

  if ((fi->flags & O_ACCMODE) != O_RDONLY) {
    LOG(WARNING) << "OzoneFSOpen: " << LOG_KEY(path) << " is not read only.";
    return -EACCES;
  }

  fi->fh = 4;

  return 0;
}

std::vector<uint8_t> readFileData(const std::string &path) {

  std::ifstream input(path, std::ios::binary);
  std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});

  return buffer;
}

int OzoneFSRead(const char *path_c_str, char *buf, size_t size, off_t offset,
                struct fuse_file_info *fi) {
  const std::filesystem::path path(path_c_str);

  LOG(INFO) << "OzoneFSRead" << LOG_KEY(path) << LOG_KEY(size)
            << LOG_KEY(offset);

  const auto &metadata = OzoneFSContext::GetContext().GetMetaData();
  if (metadata.contains(path.parent_path()) &&
      metadata.at(path.parent_path()).contains(path)) {
    const auto &file = metadata.at(path.parent_path()).at(path);
    std::string tmpfile = std::tmpnam(nullptr);
    OzoneFSContext::GetContext().CopyFile(file.path.string().substr(1),
                                          tmpfile);

    const std::vector<uint8_t> d = readFileData(tmpfile);
    const auto n = std::min(size, std::filesystem::file_size(tmpfile) - offset);

    LOG(INFO) << "OzoneFSRead: " << LOG_KEY(path) << LOG_KEY(size)
              << LOG_KEY(offset) << LOG_KEY(n);
    for (size_t i = 0; i < n; ++i) {
      buf[i] = static_cast<char>(d[i + offset]);
    }
    return n;
  }

  return 0;
}

const struct fuse_operations ozonefs_oper = {
    .getattr = OzoneFSGetattr,
    .open = OzoneFSOpen,
    .read = OzoneFSRead,
    .readdir = OzoneFSReaddir,
    .init = OzoneFSInit,
};

void show_help(const char *progname) {
  printf("usage: %s [options] <mountpoint>\n\n", progname);
  printf("File-system specific options:\n");
}
} // namespace

int main(int argc, char *argv[]) {
  int ret;
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  google::InitGoogleLogging(argv[0]);

  /* Set defaults -- we have to use strdup so that
     fuse_opt_parse can free the defaults if other
     values are specified */

  /* Parse options */
  if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
    return 1;

  /* When --help is specified, first print our own file-system
     specific help text, then signal fuse_main to show
     additional help (by adding `--help` to the options again)
     without usage: line (by setting argv[0] to the empty
     string) */
  if (options.show_help) {
    show_help(argv[0]);
    assert(fuse_opt_add_arg(&args, "--help") == 0);
    args.argv[0][0] = '\0';
  }

  CHECK_NE(std::string(options.endpoint), "");
  CHECK_NE(std::string(options.bucket_name), "");
  CHECK_NE(std::string(options.cache_dir), "");
  OzoneFSContext::InitContext(options.endpoint, options.bucket_name,
                              options.cache_dir);

  ret = fuse_main(args.argc, args.argv, &ozonefs_oper, NULL);
  fuse_opt_free_args(&args);
  return ret;
}