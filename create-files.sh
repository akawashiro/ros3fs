#! /bin/bash -eux

aws s3api --endpoint http://localhost:9878/ create-bucket --bucket=bucket1

echo "aaaaa" > /tmp/testfile
aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY  /tmp/testfile  s3://bucket1/testfile_a

echo "bbbbb" > /tmp/testfile
aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY  /tmp/testfile  s3://bucket1/testfile_b

echo "ccccc" > /tmp/testfile
aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY  /tmp/testfile  s3://bucket1/testfile_c

aws s3 --endpoint http://localhost:9878 ls s3://bucket1/testfile
