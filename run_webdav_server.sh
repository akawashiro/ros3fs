#!/bin/bash -eux

ROOT_DIR=$(dirname $0)
cd ${ROOT_DIR}/webdav
sudo docker build -t nginx_webdav .
sudo docker run --name http_backend_of_ros3fs \
    -v ${ROOT_DIR}/default.conf:/etc/nginx/conf.d/default.conf \
    --publish 8080:80 \
    --rm \
    nginx_webdav
