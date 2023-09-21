# ros3fs
ros3fs, Read Only S3 File System, is a Linux FUSE adapter for AWS S3 and S3
compatible object storage such as Apache Ozone. ros3fs focuses on speed, only speed.

## Motivation
Although Object storage systems such as AWS S3 or Apache Ozone cluster are
handy, we want to handle them as regular files sometimes. Linux FUSE enables us
to do so, and there are some works to mount S3-like systems such as
[s3fs-fuse](https://github.com/s3fs-fuse/s3fs-fuse) or [mountpoint-s3](https://github.com/awslabs/mountpoint-s3).

However, they have a critical problem. Listing files is very slow, such as
`ls` or `find`. This is because we must access S3-like systems every time
reading directory. I developed `ros3fs` to resolve this problem.

The most important insight for S3-like systems is we update data on S3-like
systems infrequently. For example, I am using an S3-like system to back up.
Although I check the contents of the backup frequently, I back up only once or
twice a month.

Under such a situation, we can cache everything locally, especially the
directory structure. Caching this information lets us speed up searching and
listing files in our storage. Of course, we must invalidate cache data
manually, but it is fine if you update your data infrequently.

## Important notice
ros3fs caches everything locally. This means the changes are not reflected if
you update your data in your bucket after ros3fs mounts the bucket. In other
words, ros3fs is suitable for accessing immutable data such as backup data,
machine learning training data, or log data.

## How to use
### Required environment
- Linux
    - I confirmed on Ubuntu 22.04 but ros3fs must work on other distributions.
- Windows
    - I confirmed on WSL2.
- Mac OS
    - I haven't confirmed but ros3fs should work on Docker for Mac.

### Build and install
Note: Use [CMAKE_INSTALL_PREFIX](https://cmake.org/cmake/help/v3.0/variable/CMAKE_INSTALL_PREFIX.html) to change the install destination.
```
# Install equivalent packages on Linux distributions other than Ubuntu.
$ sudo apt-get install -y cmake g++ git libfuse3-dev ninja-build zlib1g-dev libcurl4-openssl-dev libssl-dev ccache pkg-config
$ git clone https://github.com/akawashiro/ros3fs.git
$ cd ros3fs
$ mkdir build
$ ./build-aws-sdk-cpp.sh ./build
$ cmake -S . -B build
$ cmake --build build -- -j
$ cmake --build build -- install
```

### Mount your bucket
```
# Note: You need '=' in the command
# For example, ros3fs ./build/ros3fs_mountpoint -f -d -s --endpoint=http://localhost:9878 --bucket_name=bucket1/ --cache_dir=./build/ros3fs_cache_dir
$ ros3fs <MOUNTPOINT> -f -d --endpoint=<ENDPOINT URL> --bucket_name=<BUCKET NAME ENDS WITH '/'> --cache_dir=<CACHE DIRECTORY>
```

### Other options
```
$ ros3fs --help
usage: ./build/ros3fs [options] <mountpoint>
Example: ./build/ros3fs example_mountpoint_dir -f -d --endpoint=http://localhost:9878 \
         --bucket_name=example_bucket/ --cache_dir=example_cache_dir

ros3fs specific options. '=' is mandatory.:
--endpoint=URL         S3 endpoint (required)
--bucket_name=NAME     S3 bucket name (required)
--cache_dir=PATH       Cache directory (required)
--clear_cache          Clear cache files (optional)
--update_seconds=SECS  Update period seconds (optional)
                       Default value is 3600
--list_max_keys=KEYS   The number of keys fetched in one request (optional)
                       Default value is 1000

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

### Develop using local Ozone cluster using Docker
First, install [AWS CLI](https://docs.aws.amazon.com/ja_jp/cli/latest/userguide/getting-started-install.html).

In terminal 1,
```
$ ./launch-ozone.sh
```

In terminal 2,
```
$ aws configure set default.s3.signature_version s3v4
$ aws configure set region us-west-1
$ aws configure set aws_access_key_id "hoge"
$ aws configure set aws_secret_access_key "fuga"
$ ./create-files.sh
$ ./mount-ros3fs.sh
```

In terminal 3,
```
./run_webdav_server.sh
```

In terminal 4,
```
$ ls build/ros3fs_mountpoint
dir_a/  testfile_a  testfile_b  testfile_c
```
