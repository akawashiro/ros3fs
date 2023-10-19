#! /bin/bash -eu

cd $(git rev-parse --show-toplevel)
BUILD_DIR=$(pwd)/build_compare_with_mountpoint-s3

# Build ros3fs
if [[ ! -d ${BUILD_DIR} ]]; then
    mkdir ${BUILD_DIR}
    ./build-aws-sdk-cpp.sh ${BUILD_DIR}
    cmake -S . -B ${BUILD_DIR} 
    cmake --build ${BUILD_DIR} -- -j
fi

# Build mountpoint-s3
if [[ ! -d ${BUILD_DIR}/mountpoint-s3 ]]; then
    git clone https://github.com/awslabs/mountpoint-s3.git ${BUILD_DIR}/mountpoint-s3
    pushd ${BUILD_DIR}/mountpoint-s3
    git checkout v1.0.2
    git submodule update --init --recursive
    cargo build --release
    popd
fi

# Get OZONE_OM_IP
OZONE_OM_IP=$(sudo docker inspect --format='{{.NetworkSettings.Networks.bridge.Gateway}}' ozone-instance)

# Create bucket
aws configure set default.s3.signature_version s3v4
aws configure set region us-west-1
aws configure set aws_access_key_id "hoge"
aws configure set aws_secret_access_key "fuga"
aws s3 --endpoint http://${OZONE_OM_IP}:9878 rm s3://bucket1/ --recursive || true
aws s3api --endpoint http://${OZONE_OM_IP}:9878 delete-bucket --bucket=bucket1 || true
aws s3api --endpoint http://${OZONE_OM_IP}:9878 create-bucket --bucket=bucket1

# Create 1000 files
TMPDIR=$(mktemp -d)
for i in $(seq 1 1000)
do
    echo ${RANDOM} > ${TMPDIR}/${i}

    if [[ $((${i} % 1000)) == "0" ]]
    then
        echo "Done ${i}"
    fi
done

aws s3 --endpoint http://${OZONE_OM_IP}:9878 cp --storage-class REDUCED_REDUNDANCY --recursive ${TMPDIR} s3://bucket1/many
aws s3 --endpoint http://${OZONE_OM_IP}:9878 ls s3://bucket1/

rm -rf ${TMPDIR}

function mount_FUSEs(){
    # Mount ros3fs
    if [[ ! -d ${BUILD_DIR}/ros3fs_mountpoint ]]; then
        mkdir -p ${BUILD_DIR}/ros3fs_mountpoint
    fi
    umount ${BUILD_DIR}/ros3fs_mountpoint || true
    rm -rf ${BUILD_DIR}/ros3fs_cache_dir || true
    ${BUILD_DIR}/ros3fs ${BUILD_DIR}/ros3fs_mountpoint --endpoint=http://${OZONE_OM_IP}:9878 \
        --bucket_name=bucket1/ --cache_dir=${BUILD_DIR}/ros3fs_cache_dir --update_seconds=100 --clear_cache > /dev/null 2>&1 &
    
    # Mount s3fs-fuse
    if [[ ! -d ${BUILD_DIR}/s3fs-fuse_mountpoint ]]; then
        mkdir -p ${BUILD_DIR}/s3fs-fuse_mountpoint
    fi
    umount ${BUILD_DIR}/s3fs-fuse_mountpoint || true
    s3fs bucket1 ${BUILD_DIR}/s3fs-fuse_mountpoint  -o url=http://${OZONE_OM_IP}:9878 -o use_path_request_style
    
    # Mount mountpoint-s3
    if [[ ! -d ${BUILD_DIR}/mountpoint-s3_mountpoint ]]; then
        mkdir -p ${BUILD_DIR}/mountpoint-s3_mountpoint
    fi
    umount ${BUILD_DIR}/mountpoint-s3_mountpoint || true
    ${BUILD_DIR}/mountpoint-s3/target/release/mount-s3  --endpoint-url=http://${OZONE_OM_IP}:9878 bucket1 ${BUILD_DIR}/mountpoint-s3_mountpoint > /dev/null 2>&1
}


mount_FUSEs
echo "========== Compare grep performance without cache =========="
echo time grep -r ${BUILD_DIR}/ros3fs_mountpoint -e '123'
time grep -r ${BUILD_DIR}/ros3fs_mountpoint -e '123' > /dev/null
echo time grep -r ${BUILD_DIR}/s3fs-fuse_mountpoint -e '123'
time grep -r ${BUILD_DIR}/s3fs-fuse_mountpoint -e '123' > /dev/null
echo time grep -r ${BUILD_DIR}/mountpoint-s3_mountpoint -e '123'
time grep -r ${BUILD_DIR}/mountpoint-s3_mountpoint -e '123' > /dev/null
echo "============================================================"

mount_FUSEs
echo "========== Compare grep performance with cache =========="
hyperfine --ignore-failure --warmup 3 --runs 10 "grep -r ${BUILD_DIR}/ros3fs_mountpoint -e '123'" 2>/dev/null
hyperfine --ignore-failure --warmup 3 --runs 10 "grep -r ${BUILD_DIR}/s3fs-fuse_mountpoint -e '123'" 2>/dev/null
hyperfine --ignore-failure --warmup 3 --runs 10 "grep -r ${BUILD_DIR}/mountpoint-s3_mountpoint -e '123'" 2>/dev/null
echo "========================================================="

mount_FUSEs
echo "========== Compare find performance without cache =========="
echo time find ${BUILD_DIR}/ros3fs_mountpoint
time find ${BUILD_DIR}/ros3fs_mountpoint > /dev/null
echo time find ${BUILD_DIR}/s3fs-fuse_mountpoint
time find ${BUILD_DIR}/s3fs-fuse_mountpoint > /dev/null
echo time find ${BUILD_DIR}/mountpoint-s3_mountpoint
time find ${BUILD_DIR}/mountpoint-s3_mountpoint > /dev/null
echo "============================================================"

sleep 10
mount_FUSEs
echo "========== Compare grep performance with cache =========="
hyperfine --ignore-failure --warmup 3 --runs 10 "find ${BUILD_DIR}/ros3fs_mountpoint"
hyperfine --ignore-failure --warmup 3 --runs 10 "find ${BUILD_DIR}/s3fs-fuse_mountpoint"
hyperfine --ignore-failure --warmup 3 --runs 10 "find ${BUILD_DIR}/mountpoint-s3_mountpoint"
echo "========================================================="
