#! /bin/bash -eux

OZONEFS_MOUNTPOINT_ANSWER=$(git rev-parse --show-toplevel)/build/ozonefs_mountpoint_answer
mkdir -p ${OZONEFS_MOUNTPOINT_ANSWER}

aws s3api --endpoint http://localhost:9878/ create-bucket --bucket=bucket1

echo "aaaaa" > ${OZONEFS_MOUNTPOINT_ANSWER}/testfile_a
aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY ${OZONEFS_MOUNTPOINT_ANSWER}/testfile_a  s3://bucket1/testfile_a

echo "bbbbb" > ${OZONEFS_MOUNTPOINT_ANSWER}/testfile_b
aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY ${OZONEFS_MOUNTPOINT_ANSWER}/testfile_b s3://bucket1/testfile_b

echo "ccccc" > ${OZONEFS_MOUNTPOINT_ANSWER}/testfile_c
aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY ${OZONEFS_MOUNTPOINT_ANSWER}/testfile_c s3://bucket1/testfile_c

aws s3 --endpoint http://localhost:9878 ls s3://bucket1/testfile
