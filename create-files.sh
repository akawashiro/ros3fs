#! /bin/bash -eux
# Check http://${OZONE_OM_IP}:9876/#!/ when you encounter errors.

OZONE_OM_IP=$(docker inspect --format='{{.NetworkSettings.Networks.bridge.Gateway}}' ozone-instance)
ROS3FS_MOUNTPOINT_ANSWER=$(git rev-parse --show-toplevel)/build/ros3fs_mountpoint_answer
mkdir -p ${ROS3FS_MOUNTPOINT_ANSWER}

aws s3api --endpoint http://${OZONE_OM_IP}:9878 create-bucket --bucket=bucket1

for f in testfile_a testfile_b testfile_c dir_a/testfile_a dir_a/dir_a/testfile_a
do
    d=$(dirname ${f})
    mkdir -p ${ROS3FS_MOUNTPOINT_ANSWER}/${d}
    echo ${RANDOM} > ${ROS3FS_MOUNTPOINT_ANSWER}/${f}
    echo "aaaaaa" >> ${ROS3FS_MOUNTPOINT_ANSWER}/${f}
    aws s3 --endpoint http://${OZONE_OM_IP}:9878 cp --storage-class REDUCED_REDUNDANCY ${ROS3FS_MOUNTPOINT_ANSWER}/${f}  s3://bucket1/${f}
done

aws s3 --endpoint http://${OZONE_OM_IP}:9878 ls s3://bucket1/
