// ros3fs: Read Only S3 File System
// Copyright (C) 2023 Akira Kawata

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

private:
  const std::string endpoint_;
  const std::string bucket_name_;
  const std::filesystem::path cache_dir_;
  const std::filesystem::path meta_data_path_;
  std::shared_ptr<Directory> root_directory_;

  ROS3FSContext(const std::string &endpoint, const std::string &bucket_name,
                const std::filesystem::path &cache_dir);

  static ROS3FSContext &GetContextImpl(const std::string &endpoint,
                                       const std::string &bucket_name,
                                       const std::filesystem::path &cache_dir) {
    static ROS3FSContext context(endpoint, bucket_name, cache_dir);
    return context;
  }

  std::vector<ObjectMetaData> FetchObjectMetaData();
};
