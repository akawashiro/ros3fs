#! /bin/bash -eux
# We cannot use aws-sdk-cpp with add_subdirectory because it does not support.

rm -rf aws-sdk-cpp
AWS_SDK_CPP_INSTALL=$(git rev-parse --show-toplevel)/aws-sdk-cpp-install
mkdir -p ${AWS_SDK_CPP_INSTALL}

git clone https://github.com/aws/aws-sdk-cpp.git -b 1.11.40 --depth 1 aws-sdk-cpp
cd aws-sdk-cpp
git submodule update --init --recursive
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=${AWS_SDK_CPP_INSTALL} -G Ninja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_C_COMPILER_LAUNCHER=ccache
ninja install
