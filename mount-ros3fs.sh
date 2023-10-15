#! /bin/bash -eux

cd $(git rev-parse --show-toplevel)
cmake --build build

set +e
umount ./build/ros3fs_mountpoint
set -e

mkdir -p ./build/ros3fs_mountpoint
mkdir -p ./build/ros3fs_cache_dir
CACHE_DIR=$(mktemp -d)
OZONE_OM_IP=$(sudo docker inspect --format='{{.NetworkSettings.Networks.bridge.Gateway}}' ozone-instance)

GLOG_logtostderr=1 ./build/ros3fs ./build/ros3fs_mountpoint -f -d --endpoint=http://${OZONE_OM_IP}:9878 --bucket_name=bucket1/ --cache_dir=./build/ros3fs_cache_dir --update_seconds=100 --clear_cache
