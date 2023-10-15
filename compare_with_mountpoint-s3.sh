#! /bin/bash -eux

cd $(git rev-parse --show-toplevel)

# Build mountpoint-s3

if [[ ! -d build/mountpoint-s3 ]]; then
    git clone https://github.com/awslabs/mountpoint-s3.git build/mountpoint-s3
    pushd build/mountpoint-s3
    git checkout v1.0.2
    git submodule update --init --recursive
    cargo build --release
    popd
fi

if [[ ! -d build/mountpoint-s3_mountpoint ]]; then
    mkdir -p build/mountpoint-s3_mountpoint
fi
umount ./build/mountpoint-s3_mountpoint || true
OZONE_OM_IP=$(sudo docker inspect --format='{{.NetworkSettings.Networks.bridge.Gateway}}' ozone-instance)
./build/mountpoint-s3/target/release/mount-s3  --endpoint-url=http://${OZONE_OM_IP}:9878 bucket1 ./build/mountpoint-s3_mountpoint

hyperfine --ignore-failure "grep -Rnw ./build/ros3fs_mountpoint -e '123'"
hyperfine --ignore-failure "grep -Rnw ./build/mountpoint-s3_mountpoint -e '123'"

umount ./build/mountpoint-s3_mountpoint
