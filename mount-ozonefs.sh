#! /bin/bash -eux

pushd build
ninja
popd

set +e
umount ./build/ozonefs_mountpoint
set -e

mkdir -p ./build/ozonefs_mountpoint
GLOG_logtostderr=1 ./build/ozonefs ./build/ozonefs_mountpoint -f -d -s
