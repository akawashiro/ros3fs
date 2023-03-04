# ozonefs

## Launch ozone
- Run `./launch-ozone.sh`.
- See [https://ozone.apache.org/docs/1.3.0/start/startfromdockerhub.html](https://ozone.apache.org/docs/1.3.0/start/startfromdockerhub.html) for more details.

## Create test files
- Run `./create-files.sh`

## Use ozone client
- Run `docker command` without `sudo`. Check [https://docs.docker.com/engine/install/linux-postinstall/](https://docs.docker.com/engine/install/linux-postinstall/).
```
docker exec -it ozone-instance ./bin/ozone sh volume info /s3v
```

## FUSE libraries
- https://github.com/libfuse/libfuse
    - C
    - Active
- https://github.com/zargony/fuse-rs
    - Rust
    - Inactive
- https://github.com/jmillikin/rust-fuse
    - Rust
    - Inactive

## Log
https://qiita.com/janus_wel/items/e70695670c22a0331451
http://libfuse.github.io/doxygen/example_2hello_8c.html
```
mkdir -p ./hello_fs_mountpoint
sudo umount ./hello_fs_mountpoint
sudo ./hello_fs ./hello_fs_mountpoint --name="hoge" --contents="fuga" -f
```

# Failed attempts...

## Install AWS CLI
- https://docs.aws.amazon.com/cli/latest/userguide/getting-started-install.html
- https://docs.aws.amazon.com/cli/latest/userguide/cli-usage-output-format.html

## Build ozone
```
git clone https://github.com/apache/ozone
cd ozone
sudo apt install maven
mvn clean install -DskipTests
```

## How about s3fs
https://github.com/s3fs-fuse/s3fs-fuse
```
sudo apt install s3fs
echo hoge:hoge > ${HOME}/.passwd-s3fs
chmod 600 ${HOME}/.passwd-s3fs
mkdir ${HOME}/bucket1-s3fs
s3fs bucket1 ${HOME}/bucket1-s3fs -o passwd_file=${HOME}/.passwd-s3fs -o url=http://localhost:9878/ -o use_path_request_style
```
