#! /bin/bash -eux
# We cannot use aws-sdk-cpp with add_subdirectory because it does not support.

if [[ $# != "1" ]]
then
    echo "You must supply a build directory"
    exit 1
fi

AWS_SDK_CPP_ROOT=$(realpath ${1})

if [[ ! -d ${1} ]]
then
    echo ${AWS_SDK_CPP_ROOT} does not exist
    exit 1
fi

AWS_SDK_CPP_SOURCE=${AWS_SDK_CPP_ROOT}/aws-sdk-cpp
AWS_SDK_CPP_INSTALL=${AWS_SDK_CPP_ROOT}/aws-sdk-cpp-install
AWS_SDK_CPP_BUILD=${AWS_SDK_CPP_ROOT}/aws-sdk-cpp-build
rm -rf ${AWS_SDK_CPP_SOURCE}
rm -rf ${AWS_SDK_CPP_INSTALL}
rm -rf ${AWS_SDK_CPP_BUILD}

if [[ ! -d ${AWS_SDK_CPP_SOURCE} ]]
then
    git clone https://github.com/aws/aws-sdk-cpp.git -b 1.11.40 --depth 1 ${AWS_SDK_CPP_SOURCE}
    cd ${AWS_SDK_CPP_SOURCE}
    git submodule update --init --recursive
fi
cmake -S ${AWS_SDK_CPP_SOURCE} -B ${AWS_SDK_CPP_BUILD} -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=${AWS_SDK_CPP_INSTALL} -G Ninja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_C_COMPILER_LAUNCHER=ccache -DBUILD_SHARED_LIBS=OFF -DBUILD_ONLY="s3"
cmake --build ${AWS_SDK_CPP_BUILD} -- install
