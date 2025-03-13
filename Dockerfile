FROM ubuntu:24.04

WORKDIR /root/app

COPY ./ ./

RUN apt-get update
RUN apt install -y \
    build-essential=12.10ubuntu1 \
    cmake=3.28.3-1build7 \
    libsfml-dev=2.6.1+dfsg-2build2 \
    clang=1:18.0-59~exp2 \
    gcc=4:13.2.0-7ubuntu1 \
    ninja-build=1.11.1-2 \
    git=1:2.43.0-1ubuntu7.2
