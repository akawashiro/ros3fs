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

# Get OZONE_OM_IP
OZONE_OM_IP=$(sudo docker inspect --format='{{.NetworkSettings.Networks.bridge.Gateway}}' ozone-instance)

# Mount ros3fs
umount ./build/ros3fs_mountpoint || true
./build/ros3fs ./build/ros3fs_mountpoint --endpoint=http://${OZONE_OM_IP}:9878 \
    --bucket_name=bucket1/ --cache_dir=./build/ros3fs_cache_dir --update_seconds=100 --clear_cache &

# Mount s3fs-fuse
if [[ ! -d build/s3fs-fuse_mountpoint ]]; then
    mkdir -p build/s3fs-fuse_mountpoint
fi
umount ./build/s3fs-fuse_mountpoint || true
s3fs bucket1 build/s3fs-fuse_mountpoint  -o url=http://${OZONE_OM_IP}:9878 -o use_path_request_style

# Mount mountpoint-s3
if [[ ! -d build/mountpoint-s3_mountpoint ]]; then
    mkdir -p build/mountpoint-s3_mountpoint
fi
umount ./build/mountpoint-s3_mountpoint || true
./build/mountpoint-s3/target/release/mount-s3  --endpoint-url=http://${OZONE_OM_IP}:9878 bucket1 ./build/mountpoint-s3_mountpoint

hyperfine --ignore-failure --warmup 3 "grep -Rnw ./build/ros3fs_mountpoint -e '123'" 2>/dev/null
hyperfine --ignore-failure --warmup 3 "grep -Rnw ./build/s3fs-fuse_mountpoint -e '123'" 2>/dev/null
hyperfine --ignore-failure --warmup 3 "grep -Rnw ./build/mountpoint-s3_mountpoint -e '123'" 2>/dev/null
