FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y cmake g++ git libfuse3-dev ninja-build zlib1g-dev libcurl4-openssl-dev libssl-dev ccache
COPY . /ros3fs
WORKDIR /ros3fs
RUN ./build-aws-sdk-cpp.sh
RUN cmake -S . -B build
RUN cmake --build build -- -j
