#! /bin/bash -eux

curl http://localhost:8080/cache/ -XPUT -T CMakeLists.txt
curl http://localhost:8080/cache/CMakeLists.txt -XGET
