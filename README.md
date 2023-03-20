# ros3fs
ros3fs, Read Only S3 File System, is a Linux FUSE adapter for AWS S3 and S3
compatible object storage. ros3fs focuses on speed, only speed.

## How to build
```
git clone https://github.com/akawashiro/ros3fs.git
cd ros3fs
./build-aws-sdk-cpp.sh
mkdir build
cmake -S . -B build
cmake --build build
```

## How to use
```
# Note: You need '=' in the command
./build/ros3fs <MOUNTPOINT> -f -d -s --endpoint=<ENDPOINT URL> --bucket_name=<BUCKET NAME ENDS WITH '/''> --cache_dir=<CACHE DIRECTORY>
```

# For developers
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

## Build and use
```
$ mkdir -p ./build/ros3fs_mountpoint
$ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build -G Ninja
$ cmake --build build
$ sudo ./build/ros3fs ./build/ros3fs_mountpoint --name="hoge" --contents="fuga" -f
```

In another terminal,
```
$ sudo su
# ls build/ros3fs_mountpoint/
ls: cannot access 'build/ros3fs_mountpoint/testfile_a': No such file or directory
ls: cannot access 'build/ros3fs_mountpoint/testfile_b': No such file or directory
ls: cannot access 'build/ros3fs_mountpoint/testfile_c': No such file or directory
hoge  testfile_a  testfile_b  testfile_c
```

# Memo

## TODO
- Launch `ozone sh` from shell

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
docker exec -it ozone-instance ./bin/ozone sh key ls /s3v/bucket1
n && sudo ./ros3fs ./hello_fs_mountpoint --name="hoge" --contents="fuga" -f
```

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
