#!/bin/bash

# sq remove boost pkgs at 2020-09-27 17:19;
# sunquan update at 202-06-11 22:59;

basic_tools=(
    ascii
    cloc
    dos2unix
    gdb
    gdbm
    gettext
    gflags
    git
    # use GNU sed
    gsed
    htop
    iftop
    iperf
    iperf3
    nasm
    nghttp2
    node
    p7zip
    parallel
    readline
    tmux
    tree
    unrar
    watch
    wget
    python
    # disk usage
    gdu
    dust
    # firewall
    lulu
    # system monitor
    stats
)

macos_packages=(
    "${basic_tools[@]}"
    autoconf
    automake
    bazel
    cmake
    bwm-ng  # net tools
    nmap
    iproute2mac
    c-ares
    elixir
    erlang
    findutils
    fd
    upx
    go
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
)

cask_gui_tools=(
    aria2gui  # download file
    iina
    notion
    typora
    # mysql-client
    sequel-pro
    # tools
    wireshark
    charles
    postman
    # gif recorder
    licecap
    # hidden bar
    hiddenbar
)

brew install "${macos_packages[@]}" asciinema hugo

brew install --cask "${cask_gui_tools[@]}"

