cmake_minimum_required(VERSION 3.16)
project(ros3fs)
set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Run ./build-and-install-aws-sdk-cpp.sh before running cmake.
list(APPEND CMAKE_PREFIX_PATH
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/cmake/AWSSDK
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/cmake/aws-cpp-sdk-core
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-crt-cpp/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-http/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-mqtt/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-io/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-s3/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-sdkutils/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/s2n/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-common/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-cal/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-compression/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-auth/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-checksums/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/aws-c-event-stream/cmake
    ${CMAKE_BINARY_DIR}/aws-sdk-cpp-install/lib/cmake)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(PkgConfig)

include(third-party)
get_third_party(glog)
set(WITH_GFLAGS
    OFF
    CACHE INTERNAL "" FORCE)
set(WITH_UNWIND OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(${CMAKE_BINARY_DIR}/glog
                 ${CMAKE_BINARY_DIR}/glog EXCLUDE_FROM_ALL)
include_directories(${CMAKE_BINARY_DIR}/glog
                    ${CMAKE_BINARY_DIR}/glog/src)
add_definitions(-DC10_USE_GLOG=1)

pkg_check_modules(LIBFUSE3 REQUIRED fuse3)
message(STATUS "Found fuse3: ${LIBFUSE3_INCLUDE_DIRS} ${LIBFUSE3_LIBRARIES}")

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(ZLIB)
find_package(AWSSDK REQUIRED COMPONENTS s3)

add_executable(ros3fs ros3fs.cc sha256.cc context.cc)
install(TARGETS ros3fs DESTINATION bin)
target_link_libraries(ros3fs ${AWSSDK_LINK_LIBRARIES} ${LIBFUSE3_LIBRARIES} glog nlohmann_json::nlohmann_json  ZLIB::ZLIB)
target_include_directories(ros3fs PRIVATE ${LIBFUSE3_INCLUDE_DIRS})
target_compile_options(ros3fs PUBLIC -Wall -Werror)

if(BUILD_TESTING)
    add_executable(ls_test ls_test.cc)
    target_link_libraries(ls_test PRIVATE nlohmann_json::nlohmann_json)
    add_executable(s3-example "s3-example.cc")
    target_link_libraries(s3-example ${AWSSDK_LINK_LIBRARIES})
endif()
