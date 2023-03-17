#! /bin/bash -eux

pushd build
ninja
popd

set +e
umount ./build/ozonefs_mountpoint
set -e

GLOG_logtostderr=1 ./build/ozonefs ./build/ozonefs_mountpoint -f -d -s
