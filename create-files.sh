#! /bin/bash -eux

aws s3api --endpoint http://localhost:9878/ create-bucket --bucket=bucket1
ls -1 > /tmp/testfile
aws s3 --endpoint http://localhost:9878 cp --storage-class REDUCED_REDUNDANCY  /tmp/testfile  s3://bucket1/testfile
aws s3 --endpoint http://localhost:9878 ls s3://bucket1/testfile
