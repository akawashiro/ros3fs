// ros3fs: Read Only S3 File System
// Copyright (C) 2023 Akira Kawata

#include <cstddef>
#include <cstdint>
#include <memory>
#include <openssl/sha.h>
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

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "context.h"
#include "log.h"
#include "sha256.h"

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
namespace {
struct ROS3FSOptions {
  int show_help;
  const char *endpoint;
  const char *bucket_name;
  const char *cache_dir;
  int clear_cache;
  int update_seconds;
} ROS3FSOptions;

#define OPTION(t, p)                                                           \
  { t, offsetof(struct ROS3FSOptions, p), 1 }
const struct fuse_opt option_spec[] = {
    OPTION("-h", show_help),
    OPTION("--help", show_help),
    OPTION("--endpoint=%s", endpoint),
    OPTION("--bucket_name=%s", bucket_name),
    OPTION("--cache_dir=%s", cache_dir),
    OPTION("--clear_cache", clear_cache),
    OPTION("--update_seconds=%d", update_seconds),
    FUSE_OPT_END};

void show_help(const char *progname) {
  std::cout
      << "usage: " << progname << " [options] <mountpoint>" << std::endl
      << "Example: " << progname
      << " example_mountpoint_dir -f -d "
         "--endpoint=http://localhost:9878 \\"
      << std::endl
      << "         --bucket_name=example_bucket/ "
         "--cache_dir=example_cache_dir"
      << std::endl
      << std::endl
      << "ros3fs specific options. '=' is mandatory.:" << std::endl
      << "--endpoint=URL         S3 endpoint (required)" << std::endl
      << "--bucket_name=NAME     S3 bucket name (required)" << std::endl
      << "--cache_dir=PATH       Cache directory (required)" << std::endl
      << "--clear_cache          Clear cache files (optional)" << std::endl
      << "--update_seconds=SECONDS" << std::endl
      << "                       Update period seconds (optional)"
      << std::endl
      << "                       Default value is 3600" << std::endl
      << std::endl
      << std::endl
      << "FUSE specific options:" << std::endl
      << "-d, -odebug" << std::endl
      << "    Causes debug information for subsequent FUSE library calls"
      << std::endl
      << "    to be output to stderr. Implies -f." << std::endl
      << "-f" << std::endl
      << "    If this is specified then fg will be set to 1 on success. "
      << std::endl
      << "    This flag indicates that the file system should not detach from "
      << std::endl
      << "    the controlling terminal and run in the foreground." << std::endl
      << "-h, --help, -ho" << std::endl
      << "    Print usage information for the options supported by "
      << std::endl
      << "    fuse_parse_cmdline()." << std::endl
      << "-s" << std::endl
      << "    If this is specified then mt will be set to 0 on success. "
      << std::endl
      << "    This flag indicates that the file system should be run "
      << std::endl
      << "    in multi-threaded mode. -s is currently ignored and mt will "
         "always be 0."
      << std::endl;
}

void *ROS3FSInit(struct fuse_conn_info *conn, struct fuse_config *cfg) {
  (void)conn;
  cfg->kernel_cache = 1;

  return NULL;
}

int ROS3FSGetattr(const char *path_c_str, struct stat *stbuf,
                  struct fuse_file_info *fi) {
  LOG(INFO) << "ROS3FSGetattr" << LOG_KEY(path_c_str);

  (void)fi;

  memset(stbuf, 0, sizeof(struct stat));

  // Check https://linuxjm.osdn.jp/html/LDP_man-pages/man2/stat.2.html for
  // S_IFDIR and S_IFREG.

  const std::filesystem::path path(path_c_str);

  const auto meta = ROS3FSContext::GetContext().GetAttr(path);
  if (meta.has_value()) {
    if (meta.value().type == FileType::kDirectory) {
      stbuf->st_mode = S_IFDIR | 0444;
      stbuf->st_nlink = 2;

      // TODO: This is platform dependent.
      stbuf->st_atime = meta.value().unix_time_millis / 1000;
      stbuf->st_mtime = meta.value().unix_time_millis / 1000;
      stbuf->st_ctime = meta.value().unix_time_millis / 1000;

      LOG(INFO) << "ROS3FSGetattr: " << LOG_KEY(path) << " is a directory.";
    } else {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = meta.value().size;

      // TODO: This is platform dependent.
      stbuf->st_atime = meta.value().unix_time_millis / 1000;
      stbuf->st_mtime = meta.value().unix_time_millis / 1000;
      stbuf->st_ctime = meta.value().unix_time_millis / 1000;

      LOG(INFO) << "ROS3FSGetattr: " << LOG_KEY(path) << " is a normal file.";
    }
    return 0;
  } else {
    LOG(INFO) << "ROS3FSGetattr: " << LOG_KEY(path) << " does not exist.";
    return -ENOENT;
  }
}

int ROS3FSReaddir(const char *path_c_str, void *buf, fuse_fill_dir_t filler,
                  off_t offset, struct fuse_file_info *fi,
                  enum fuse_readdir_flags flags) {
  (void)offset;
  (void)fi;
  (void)flags;

  const std::filesystem::path path(path_c_str);
  LOG(INFO) << "ROS3FSReaddir" << LOG_KEY(path);

  filler(buf, ".", NULL, 0, static_cast<fuse_fill_dir_flags>(0));
  filler(buf, "..", NULL, 0, static_cast<fuse_fill_dir_flags>(0));

  const auto &metas = ROS3FSContext::GetContext().ReadDirectory(path);
  for (const auto &m : metas) {
    LOG(INFO) << "ROS3FSReaddir: Found " << LOG_KEY(m.name) << " in "
              << LOG_KEY(path);
    filler(buf, m.name.c_str(), NULL, 0, static_cast<fuse_fill_dir_flags>(0));
  }

  return 0;
}

int ROS3FSOpen(const char *path, struct fuse_file_info *fi) {
  LOG(INFO) << "ROS3FSOpen" << LOG_KEY(path);

  if ((fi->flags & O_ACCMODE) != O_RDONLY) {
    LOG(WARNING) << "ROS3FSOpen: " << LOG_KEY(path) << " is not read only.";
    return -EACCES;
  }

  fi->fh = 4;

  return 0;
}

int ROS3FSRead(const char *path_c_str, char *buf, size_t size, off_t offset,
               struct fuse_file_info *fi) {
  const std::filesystem::path path(path_c_str);

  LOG(INFO) << "ROS3FSRead" << LOG_KEY(path) << LOG_KEY(size)
            << LOG_KEY(offset);

  std::optional<FileMetaData> meta = ROS3FSContext::GetContext().GetAttr(path);
  if (meta.has_value() && meta.value().type == FileType::kFile) {
    const std::vector<uint8_t> d =
        ROS3FSContext::GetContext().GetFileContents(path);
    const auto n = std::min(size, d.size() - offset);

    LOG(INFO) << "ROS3FSRead: " << LOG_KEY(path) << LOG_KEY(size)
              << LOG_KEY(offset) << LOG_KEY(n);
    for (size_t i = 0; i < n; ++i) {
      buf[i] = static_cast<char>(d[i + offset]);
    }
    return n;
  }

  return 0;
}

const struct fuse_operations ozonefs_oper = {
    .getattr = ROS3FSGetattr,
    .open = ROS3FSOpen,
    .read = ROS3FSRead,
    .readdir = ROS3FSReaddir,
    .init = ROS3FSInit,
};
} // namespace

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);

  int ret;
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  /* Set defaults -- we have to use strdup so that
     fuse_opt_parse can free the defaults if other
     values are specified */

  ROS3FSOptions.bucket_name = strdup("");
  ROS3FSOptions.endpoint = strdup("");
  ROS3FSOptions.cache_dir = strdup("");

  /* Parse ROS3FSOptions */
  if (fuse_opt_parse(&args, &ROS3FSOptions, option_spec, NULL) == -1)
    return 1;

  /* When --help is specified, first print our own file-system
     specific help text, then signal fuse_main to show
     additional help (by adding `--help` to the ROS3FSOptions again)
     without usage: line (by setting argv[0] to the empty
     string) */
  if (ROS3FSOptions.show_help) {
    show_help(argv[0]);
    assert(fuse_opt_add_arg(&args, "--help") == 0);
    args.argv[0][0] = '\0';
    exit(0);
  }

  if (std::string(ROS3FSOptions.endpoint) == "") {
    std::cerr << "--endpoint is not specified." << std::endl << std::endl;
    show_help(argv[0]);
    exit(1);
  }

  if (std::string(ROS3FSOptions.bucket_name) == "") {
    std::cerr << "--bucket_name is not specified." << std::endl << std::endl;
    show_help(argv[0]);
    exit(1);
  }

  if (std::string(ROS3FSOptions.cache_dir) == "") {
    std::cerr << "--cache_dir is not specified." << std::endl << std::endl;
    show_help(argv[0]);
    exit(1);
  }

  CHECK_NE(std::string(ROS3FSOptions.endpoint), "");
  CHECK_NE(std::string(ROS3FSOptions.bucket_name), "");
  CHECK_NE(std::string(ROS3FSOptions.cache_dir), "");

  std::filesystem::path cache_dir_root(ROS3FSOptions.cache_dir);
  std::filesystem::path cache_dir(
      cache_dir_root / GetSHA256(std::string(ROS3FSOptions.endpoint) +
                                 ROS3FSOptions.bucket_name));
  std::filesystem::create_directories(cache_dir_root);
  std::filesystem::create_directories(cache_dir);

  const bool clear_cache = ROS3FSOptions.clear_cache;

  constexpr int defaultUpdateSeconds = 3600;
  const int update_seconds = ROS3FSOptions.update_seconds > 0
                                 ? ROS3FSOptions.update_seconds
                                 : defaultUpdateSeconds;

  ROS3FSContext::InitContext(ROS3FSOptions.endpoint, ROS3FSOptions.bucket_name,
                             update_seconds, cache_dir, clear_cache);

  ret = fuse_main(args.argc, args.argv, &ozonefs_oper, NULL);
  fuse_opt_free_args(&args);
  return ret;
}
