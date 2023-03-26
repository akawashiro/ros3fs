// ros3fs: Read Only S3 File System
// Copyright (C) 2023 Akira Kawata

#include "context.h"
#include "glog/logging.h"

#include <cstddef>
#include <memory>
#include <nlohmann/json.hpp>

#include <aws/core/Aws.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/HeadObjectRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>

#include <optional>

#include "sha256.h"

namespace {

std::string
SerializeObjectMetaData(const std::vector<ObjectMetaData> &meta_datas) {
  nlohmann::json j;
  for (const auto &md : meta_datas) {
    nlohmann::json j2;
    j2["path"] = md.path;
    j2["size"] = md.size;
    j.push_back(j2);
  }
  return j.dump();
}

std::vector<ObjectMetaData> DeserializeObjectMetaData(const std::string &json) {
  std::vector<ObjectMetaData> meta_datas;
  nlohmann::json j = nlohmann::json::parse(json);
  for (const auto &j2 : j) {
    ObjectMetaData md;
    md.path = std::filesystem::path(j2["path"]);
    md.size = j2["size"];
    meta_datas.push_back(md);
  }
  return meta_datas;
}
} // namespace

void ROS3FSContext::CopyFile(const std::string &src, const std::string &dst) {
  {
    Aws::Client::ClientConfiguration config;
    config.endpointOverride = endpoint_;
    Aws::S3::S3Client client(config);

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
}

std::vector<ObjectMetaData> ROS3FSContext::FetchObjectMetaData() {
  if (std::filesystem::exists(meta_data_path_)) {
    LOG(INFO) << "Loading meta data from " << meta_data_path_;
    std::ifstream ifs(meta_data_path_);
    std::string json((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
    return DeserializeObjectMetaData(json);
  }

  std::set<std::filesystem::path> file_and_dirs;
  std::vector<ObjectMetaData> result_files;

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
          result_files.push_back(ObjectMetaData{
              .path = "/" + object.GetKey(),
              .size = static_cast<uint64_t>(object.GetSize()),
          });
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

  LOG(INFO) << "Save metadata to " << meta_data_path_;
  std::ofstream ofs(meta_data_path_);
  ofs << SerializeObjectMetaData(result_files);

  return result_files;
}

ROS3FSContext::ROS3FSContext(const std::string &endpoint,
                             const std::string &bucket_name,
                             const std::filesystem::path &cache_dir)
    : endpoint_(endpoint), bucket_name_(bucket_name),
      cache_dir_(std::filesystem::canonical(cache_dir)),
      lock_dir_(std::filesystem::canonical(cache_dir) / "lock"),
      meta_data_path_(
          std::filesystem::canonical(cache_dir) /
          ("ros3fs_meta_data_" + GetSHA256(endpoint + bucket_name) + ".json")) {
  CHECK_NE(endpoint, "");
  CHECK_NE(bucket_name, "");
  CHECK(std::filesystem::exists(cache_dir));
  LOG(INFO) << "ROS3FSContext initialized with endpoint=" << endpoint
            << " bucket_name=" << bucket_name
            << " cache_dir=" << std::filesystem::canonical(cache_dir);

  CHECK(std::filesystem::create_directory(lock_dir_))
      << "Failed to create lock directory: " << lock_dir_
      << ". If you don't run other process to mount this bucket, please remove "
         "this directory and try again.";

  LOG(INFO) << "Initialize AWS SDK API";
  // The AWS SDK for C++ must be initialized by calling Aws::InitAPI.
  Aws::InitAPI(sdk_options_);

  std::vector<ObjectMetaData> meta_datas = FetchObjectMetaData();

  root_directory_ = std::make_shared<Directory>();

  root_directory_->self.name = "/";
  root_directory_->self.size = 0;
  root_directory_->self.type = FileType::kDirectory;

  LOG(INFO) << LOG_KEY(meta_datas.size());
  for (const auto &md : meta_datas) {
    // TODO: Handle empty directory.
    std::vector<std::filesystem::path> dirs(md.path.begin(), md.path.end());
    CHECK_GE(dirs.size(), static_cast<size_t>(1));
    CHECK_EQ(dirs[0], "/");

    std::shared_ptr<Directory> current_dir = root_directory_;
    for (size_t i = 1; i < dirs.size(); i++) {
      if (dirs.size() != i + 1) {
        // Directory
        if (!current_dir->directories.contains(dirs[i])) {
          current_dir->directories[dirs[i]] = std::make_shared<Directory>(
              Directory{.self = FileMetaData{.name = dirs[i],
                                             .size = 0,
                                             .type = FileType::kDirectory}});
        }
        current_dir = current_dir->directories[dirs[i]];
      } else {
        // File
        CHECK(!current_dir->directories.contains(dirs[i]));
        current_dir->directories[dirs[i]] = std::make_shared<Directory>(
            Directory{.self = FileMetaData{.name = dirs[i],
                                           .size = md.size,
                                           .type = FileType::kFile}});
      }
    }
  }

  sdk_options_.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
}

ROS3FSContext::~ROS3FSContext() {
  LOG(INFO) << "Shutdown AWS SDK API";
  // Before the application terminates, the SDK must be shut down.
  ShutdownAPI(sdk_options_);

  CHECK(std::filesystem::remove(lock_dir_))
      << "Failed to remove lock directory: " << lock_dir_;
}

std::vector<FileMetaData>
ROS3FSContext::ReadDirectory(const std::filesystem::path &path) {
  LOG(INFO) << "ReadDirectory " << LOG_KEY(path);

  std::vector<std::filesystem::path> dirs(path.begin(), path.end());
  CHECK_GE(dirs.size(), static_cast<size_t>(1));
  CHECK_EQ(dirs[0], "/");

  std::shared_ptr<Directory> current_dir = root_directory_;
  for (size_t i = 0; i < dirs.size(); i++) {
    VLOG(3) << LOG_KEY(dirs.size()) << LOG_KEY(i) << LOG_KEY(dirs[i])
            << LOG_KEY(current_dir->directories.size())
            << LOG_KEY(current_dir->self.name);
    if (dirs.size() == i + 1) {
      std::vector<FileMetaData> result;
      for (const auto &dir : current_dir->directories) {
        result.emplace_back(dir.second->self);
      }
      return result;
    }

    if (!current_dir->directories.contains(dirs[i + 1])) {
      return {};
    } else {
      VLOG(3) << LOG_KEY(dirs[i + 1]);
      current_dir = current_dir->directories.at(dirs[i + 1]);
    }
  }

  return {};
}

std::optional<FileMetaData>
ROS3FSContext::GetAttr(const std::filesystem::path &path) {
  std::vector<std::filesystem::path> dirs(path.begin(), path.end());
  CHECK_GE(dirs.size(), static_cast<size_t>(1));
  CHECK_EQ(dirs[0], "/");

  std::shared_ptr<Directory> current_dir = root_directory_;
  for (size_t i = 0; i < dirs.size(); i++) {
    VLOG(3) << LOG_KEY(dirs[i]);
    if (dirs.size() == i + 1) {
      return current_dir->self;
    }

    VLOG(3) << LOG_KEY(dirs[i + 1]);
    if (!current_dir->directories.contains(dirs[i + 1])) {
      return std::nullopt;
    } else {
      current_dir = current_dir->directories.at(dirs[i + 1]);
    }
  }

  return std::nullopt;
}
