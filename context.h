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
  static ROS3FSContext &GetContext() { return GetContextImpl("", "", ""); }
  static void InitContext(const std::string &endpoint,
                          const std::string &bucket_name,
                          const std::filesystem::path &cache_dir) {
    GetContextImpl(endpoint, bucket_name, cache_dir);
  }

  std::vector<FileMetaData> ReadDirectory(const std::filesystem::path &path);
  std::optional<FileMetaData> GetAttr(const std::filesystem::path &path);

  void CopyFile(const std::string &src, const std::string &dst);
  std::filesystem::path cache_dir() const { return cache_dir_; }

private:
  const std::string endpoint_;
  const std::string bucket_name_;
  const std::filesystem::path cache_dir_;
  const std::filesystem::path lock_dir_;
  const uint64_t update_metadata_seconds_;

  // You must get meta_data_mutex_ before accessing root_directory_ and meta_data_path_.
  std::mutex meta_data_mutex_;
  std::shared_ptr<Directory> root_directory_;
  const std::filesystem::path meta_data_path_;

  Aws::SDKOptions sdk_options_;
  std::thread update_metadata_loop_thread_;

  ROS3FSContext(const std::string &endpoint, const std::string &bucket_name,
                const std::filesystem::path &cache_dir);
  ~ROS3FSContext();

  static ROS3FSContext &GetContextImpl(const std::string &endpoint,
                                       const std::string &bucket_name,
                                       const std::filesystem::path &cache_dir) {
    static ROS3FSContext context(endpoint, bucket_name, cache_dir);
    return context;
  }

  void InitMetaData();
  std::vector<ObjectMetaData> FetchObjectMetaDataFromS3();
  void UpdateMetaDataLoop();
  void UpdateRootDir(const std::vector<ObjectMetaData> &meta_datas);
};
