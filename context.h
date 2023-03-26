// ros3fs: Read Only S3 File System
// Copyright (C) 2023 Akira Kawata

#include <aws/core/Aws.h>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "log.h"

enum class FileType { kFile, kDirectory };

struct FileMetaData {
  std::string name;
  uint64_t size;
  FileType type;
};

struct ObjectMetaData {
  std::filesystem::path path;
  uint64_t size;
};

struct Directory {
  FileMetaData self;
  std::unordered_map<std::string, std::shared_ptr<Directory>> directories;
};

class ROS3FSContext {
public:
  ROS3FSContext(ROS3FSContext const &) = delete;
  void operator=(ROS3FSContext const &) = delete;
  static ROS3FSContext &GetContext() {
    return GetContextImpl("", "", 0, "", false);
  }
  static void InitContext(const std::string &endpoint,
                          const std::string &bucket_name,
                          const int update_metadata_seconds,
                          const std::filesystem::path &cache_dir,
                          const bool clear_cache) {
    GetContextImpl(endpoint, bucket_name, update_metadata_seconds, cache_dir,
                   clear_cache);
  }

  std::vector<FileMetaData> ReadDirectory(const std::filesystem::path &path);
  std::optional<FileMetaData> GetAttr(const std::filesystem::path &path);

  void CopyFile(const std::string &src, const std::string &dst);
  std::filesystem::path cache_dir() const { return cache_dir_; }

private:
  const std::string endpoint_;
  const std::string bucket_name_;
  const std::filesystem::path cache_dir_;
  const bool clear_cache_;
  const std::filesystem::path lock_dir_;
  const uint64_t update_metadata_seconds_;

  // You must get meta_data_mutex_ before accessing root_directory_ and
  // meta_data_path_.
  std::mutex meta_data_mutex_;
  std::shared_ptr<Directory> root_directory_;
  const std::filesystem::path meta_data_path_;

  Aws::SDKOptions sdk_options_;

  // TODO: We don't need to use atomic<bool> here.
  std::atomic<bool> update_metadata_loop_stop_ = false;
  std::mutex update_metadata_loop_mtx_;
  std::condition_variable update_metadata_loop_cv_;
  std::thread update_metadata_loop_thread_;

  ROS3FSContext(const std::string &endpoint, const std::string &bucket_name,
                const int update_metadata_seconds,
                const std::filesystem::path &cache_dir, const bool clear_cache);
  ~ROS3FSContext();

  static ROS3FSContext &GetContextImpl(const std::string &endpoint,
                                       const std::string &bucket_name,
                                       const int update_metadata_seconds,
                                       const std::filesystem::path &cache_dir,
                                       const bool clear_cache) {
    static ROS3FSContext context(endpoint, bucket_name, update_metadata_seconds,
                                 cache_dir, clear_cache);
    return context;
  }

  void InitMetaData();
  std::vector<ObjectMetaData> FetchObjectMetaDataFromS3();
  void UpdateMetaDataLoop();
  void UpdateRootDir(const std::vector<ObjectMetaData> &meta_datas);
};
