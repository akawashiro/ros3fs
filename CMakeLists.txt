cmake_minimum_required(VERSION 3.16)
project(sloader)

enable_testing()
set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(PkgConfig)

include(third-party)
get_third_party(glog)
set(WITH_GFLAGS
    OFF
    CACHE INTERNAL "" FORCE)
set(WITH_UNWIND OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/glog
                 ${CMAKE_CURRENT_BINARY_DIR}/glog EXCLUDE_FROM_ALL)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/glog
                    ${CMAKE_CURRENT_BINARY_DIR}/glog/src)
add_definitions(-DC10_USE_GLOG=1)

pkg_check_modules(LIBFUSE3 REQUIRED fuse3)
message(STATUS "Found fuse3: ${LIBFUSE3_INCLUDE_DIRS} ${LIBFUSE3_LIBRARIES}")

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(hello_fs hello.cc)
target_link_libraries(hello_fs ${LIBFUSE3_LIBRARIES} glog)
target_include_directories(hello_fs PRIVATE ${LIBFUSE3_INCLUDE_DIRS})
target_compile_options(hello_fs PUBLIC -Wall)

add_executable(ozonefs ozonefs.cc)
target_link_libraries(ozonefs ${LIBFUSE3_LIBRARIES} glog  nlohmann_json::nlohmann_json)
target_include_directories(ozonefs PRIVATE ${LIBFUSE3_INCLUDE_DIRS})
target_compile_options(ozonefs PUBLIC -Wall)

add_executable(ls_test ls_test.cc)
target_link_libraries(ls_test PRIVATE nlohmann_json::nlohmann_json)
