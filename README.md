# ros3fs
ros3fs, Read Only S3 File System, is a Linux FUSE adapter for AWS S3 and S3
compatible object storage such as Apache Ozone. ros3fs focuses on speed, only speed.

## Important notice
ros3fs caches everything locally. This means the changes are not reflected if
you update your data in your bucket after ros3fs mounts the bucket. In other
words, ros3fs is suitable for accessing immutable data such as backup data,
machine learning training data, or log data.

# Required environment
- Linux
    - I confirmed on Ubuntu 22.04 but ros3fs must work on other distributions.
- Windows
    - I confirmed on WSL2.
- Mac OS
    - I haven't confirmed but ros3fs should work on Docker for Mac.

## How to build and install
Note: Use [CMAKE_INSTALL_PREFIX](https://cmake.org/cmake/help/v3.0/variable/CMAKE_INSTALL_PREFIX.html) to change the install destination.
```
# Install equivalent packages on Linux distributions other than Ubuntu.
sudo apt-get install -y cmake g++ git libfuse3-dev ninja-build zlib1g-dev libcurl4-openssl-dev libssl-dev ccache pkg-config
git clone https://github.com/akawashiro/ros3fs.git
cd ros3fs
./build-aws-sdk-cpp.sh
cmake -S . -B build
cmake --build build -- -j
cmake --build build -- install
```

## How to use
### Mount your bucket
```
# Note: You need '=' in the command
# For example, ros3fs ./build/ros3fs_mountpoint -f -d -s --endpoint=http://localhost:9878 --bucket_name=bucket1/ --cache_dir=./build/ros3fs_cache_dir
ros3fs <MOUNTPOINT> -f -d -s --endpoint=<ENDPOINT URL> --bucket_name=<BUCKET NAME ENDS WITH '/'> --cache_dir=<CACHE DIRECTORY>
```

### Other options
```
ros3fs --help
usage: ./build/ros3fs [options] <mountpoint>
Example: ./build/ros3fs example_mountpoint_dir -f -d --endpoint=http://localhost:9878 \
         --bucket_name=example_bucket/ --cache_dir=example_cache_dir

ros3fs specific options. '=' is mandatory.:
--endpoint=URL         S3 endpoint (required)
--bucket_name=NAME     S3 bucket name (required)
--cache_dir=PATH       Cache directory (required)
--clear_cache          Clear cache files (optional)
--update_seconds=SECONDS
                       Update metadata period seconds (optional)
                       Default value is 3600


FUSE specific options:
-d, -odebug
    Causes debug information for subsequent FUSE library calls
    to be output to stderr. Implies -f.
-f
    If this is specified then fg will be set to 1 on success. 
    This flag indicates that the file system should not detach from 
    the controlling terminal and run in the foreground.
-h, --help, -ho
    Print usage information for the options supported by 
    fuse_parse_cmdline().
-s
    If this is specified then mt will be set to 0 on success. 
    This flag indicates that the file system should be run 
    in multi-threaded mode. -s is currently ignored and mt will always be 0.
```
