FROM mcr.microsoft.com/devcontainers/universal:2-linux

RUN apt-get update
RUN apt-get install libsfml-dev -y
RUN apt install -y pcmanfm
RUN apt install -y clang-format
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN apt install -y g++-11

ENV DISPLAY=host.docker.internal:0.0