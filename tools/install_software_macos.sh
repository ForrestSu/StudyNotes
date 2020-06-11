#!/bin/bash

# sunquan update at 202-06-11 22:59
basic_tools=(
    ascii
    dos2unix
    gdb
    gdbm
    gettext
    gflags
    git
    htop
    iftop
    iperf
    nasm
    nghttp2
    node
    p7zip
    parallel
    python
    readline
    tmux
    tree
    unrar
    watch
    wget
)

macos_packages=(
    "${basic_tools[@]}"
    autoconf
    automake
    bazel
    cmake
    boost
    bwm-ng
    iproute2mac
    c-ares
    elixir
    erlang
    findutils
    go     # google
    grpc
    protobuf
    graphviz
    jemalloc
    libssh
    libtool
    libuv
    maven
    nginx
    openssl
    redis
    spdlog
    sqlite
    tinyxml2 # config
    yaml-cpp
    zeromq
)

cask_gui_tools=(
    aria2gui  # download file 
    copyq     # clipboard enhance
)

brew install "${macos_packages[@]}" bochs hugo

brew cask install  "${cask_gui_tools[@]}"

