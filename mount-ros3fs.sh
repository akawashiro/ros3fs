#! /bin/bash -eux

cd $(git rev-parse --show-toplevel)
pushd build
ninja
popd

set +e
umount ./build/ros3fs_mountpoint
set -e

mkdir -p ./build/ros3fs_mountpoint
mkdir -p ./build/ros3fs_cache_dir
GLOG_logtostderr=1 ./build/ros3fs ./build/ros3fs_mountpoint -f -d -s --endpoint=http://localhost:9878 --bucket_name=bucket1/ --cache_dir=./build/ros3fs_cache_dir
