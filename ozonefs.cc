// ozonefs: FUSE filesystem for Ozone
// Copyright (C) 2023 Akira Kawata
// This program can be distributed under the terms of the GNU GPLv2.

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
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

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
} options;

#define OPTION(t, p)                                                           \
  { t, offsetof(struct options, p), 1 }
const struct fuse_opt option_spec[] = {
    OPTION("-h", show_help), OPTION("--help", show_help), FUSE_OPT_END};

void *ozonefs_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
  (void)conn;
  cfg->kernel_cache = 1;
  return NULL;
}

std::vector<std::string> ListAllFiles() {
  using json = nlohmann::json;

  std::string exec_out = std::tmpnam(nullptr);
  std::string exec_cmd = std::string("docker exec -it ozone-instance "
                                     "./bin/ozone sh key ls /s3v/bucket1 > ") +
                         exec_out;

  LOG(INFO) << "exec_cmd: " << exec_cmd << " exec_out: " << exec_out;

  CHECK_EQ(std::system(exec_cmd.c_str()), 0);

  std::ifstream f(exec_out);
  json data = json::parse(f);

  std::vector<std::string> result;
  for (json::iterator it = data.begin(); it != data.end(); ++it) {
    result.push_back((*it)["name"]);
  }
  return result;
}

void CopyFile(const std::string &src, const std::string &dst) {
  std::string ozone_get_cmd =
      std::string("docker exec -it ozone-instance ./bin/ozone sh key get "
                  "/s3v/bucket1" +
                  src + " /tmp/" + dst);

  LOG(INFO) << "ozone_get_cmd: " << ozone_get_cmd;
  CHECK_EQ(std::system(ozone_get_cmd.c_str()), 0);

  std::string docker_cp_cmd =
      std::string("docker cp ozone-instance:/tmp/" + src + " /tmp" + dst);

  LOG(INFO) << "docker_cp_cmd: " << docker_cp_cmd;
  CHECK_EQ(std::system(docker_cp_cmd.c_str()), 0);
}

int ozonefs_getattr(const char *path, struct stat *stbuf,
                    struct fuse_file_info *fi) {
  LOG(INFO) << "ozonefs_getattr" << LOG_KEY(path);

  (void)fi;

  memset(stbuf, 0, sizeof(struct stat));

  // Check https://linuxjm.osdn.jp/html/LDP_man-pages/man2/stat.2.html for
  // S_IFDIR and S_IFREG.

  const std::string path_str(path);
  if (path_str == "/") {
    stbuf->st_mode = S_IFDIR | 0444;
    stbuf->st_nlink = 2;
    LOG(INFO) << "ozonefs_getattr: " << LOG_KEY(path_str) << " is the root.";
    return 0;
  }

  const auto all_files = ListAllFiles();
  for (const auto &f : all_files) {
    if ("/" + f == path_str) {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 2;
      LOG(INFO) << "ozonefs_getattr: " << LOG_KEY(path_str)
                << " is a normal file.";
      return 0;
    }
  }

  LOG(INFO) << "ozonefs_getattr: " << LOG_KEY(path_str) << " does not exist.";
  return -ENOENT;
}

int ozonefs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                    off_t offset, struct fuse_file_info *fi,
                    enum fuse_readdir_flags flags) {
  (void)offset;
  (void)fi;
  (void)flags;

  LOG(INFO) << "ozonefs_readdir" << LOG_KEY(path);

  if (strcmp(path, "/") != 0)
    return -ENOENT;

  filler(buf, ".", NULL, 0, static_cast<fuse_fill_dir_flags>(0));
  filler(buf, "..", NULL, 0, static_cast<fuse_fill_dir_flags>(0));

  const auto &files = ListAllFiles();
  for (const auto &file : files) {
    filler(buf, file.c_str(), NULL, 0, static_cast<fuse_fill_dir_flags>(0));
  }

  return 0;
}

int ozonefs_open(const char *path, struct fuse_file_info *fi) {
  LOG(INFO) << "ozonefs_open" << LOG_KEY(path);

  if ((fi->flags & O_ACCMODE) != O_RDONLY){
    LOG(WARNING) << "ozonefs_open: " << LOG_KEY(path) << " is not read only.";
    return -EACCES;
  }

  return 0;
}

int ozonefs_read(const char *path, char *buf, size_t size, off_t offset,
                 struct fuse_file_info *fi) {
  const std::string path_str(path);
  LOG(INFO) << "ozonefs_read" << LOG_KEY(path_str);

  const auto &files = ListAllFiles();
  for (const auto &file : files) {
      if(file == path_str) {
          CopyFile(file, path_str);
      }
  }

  (void)fi;

  size = 0;

  return size;
}

const struct fuse_operations ozonefs_oper = {
    .getattr = ozonefs_getattr,
    .open = ozonefs_open,
    .read = ozonefs_read,
    .readdir = ozonefs_readdir,
    .init = ozonefs_init,
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

  ret = fuse_main(args.argc, args.argv, &ozonefs_oper, NULL);
  fuse_opt_free_args(&args);
  return ret;
}
