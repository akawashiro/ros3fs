#! /bin/bash -eux
# Check http://localhost:9876/#!/ when you encounter errors.

OZONEFS_MOUNTPOINT_ANSWER=$(git rev-parse --show-toplevel)/build/ozonefs_mountpoint_answer
mkdir -p ${OZONEFS_MOUNTPOINT_ANSWER}

aws s3api --endpoint http://localhost:9878 create-bucket --bucket=bucket1

for f in testfile_a testfile_b testfile_c dir_a/testfile_a dir_a/dir_a/testfile_a
do
    d=$(dirname ${f})
    mkdir -p ${OZONEFS_MOUNTPOINT_ANSWER}/${d}
    echo ${RANDOM} > ${OZONEFS_MOUNTPOINT_ANSWER}/${f}
    echo "aaaaaa" >> ${OZONEFS_MOUNTPOINT_ANSWER}/${f}
    aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY ${OZONEFS_MOUNTPOINT_ANSWER}/${f}  s3://bucket1/${f}
done

aws s3 --endpoint http://localhost:9878 ls s3://bucket1/
