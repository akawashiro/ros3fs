# ros3fs
ros3fs, Read Only S3 File System, is a Linux FUSE adapter for AWS S3 and S3
compatible object storage such as Apache Ozone. ros3fs focuses on speed, only speed.

## How to build
```
git clone https://github.com/akawashiro/ros3fs.git
cd ros3fs
./build-aws-sdk-cpp.sh
mkdir build
cmake -S . -B build
cmake --build build -- -j
```

## How to use
```
# Note: You need '=' in the command
# For example, ./build/ros3fs ./build/ros3fs_mountpoint -f -d -s --endpoint=http://localhost:9878 --bucket_name=bucket1/ --cache_dir=./build/ros3fs_cache_dir
./build/ros3fs <MOUNTPOINT> -f -d -s --endpoint=<ENDPOINT URL> --bucket_name=<BUCKET NAME ENDS WITH '/'> --cache_dir=<CACHE DIRECTORY>
```
