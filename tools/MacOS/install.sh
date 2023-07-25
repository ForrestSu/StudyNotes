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
    watch
    wget
    python
    # remote sync
    rsync
    # disk usage
    gdu
    dust
    asciinema
    # blog
    hugo
)

macos_packages=(
    "${basic_tools[@]}"
    autoconf
    automake
    bazel
    cmake
    # net tools
    bwm-ng
    nmap
    iproute2mac
    c-ares
    findutils
    # code format
    clang-format
    fd
    upx
    go
    grpc
    protobuf
    protoc-gen-go
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
)

cask_gui_tools=(
    aria2gui  # download file
    notion
    # mysql-client
    sequel-pro
    # tools
    wireshark
    charles
    postman
    # hidden bar
    hiddenbar
    # firewall
    lulu
    # system monitor
    stats
    google-chrome
    # dev tools
    visual-studio-code
    beyond-compare # gui-diff
    docker
    goland
)

media_gui_tools=(
    iina
    # gif recorder
    licecap
    # record screen with audio
    blackhole-16ch
    # obs-studio: recording screen
    obs
)

office_gui_tools=(
    # sogou input
    sogouinput
    # 腾讯会议
    tencent-meeting
)


brew install "${macos_packages[@]}"

brew install --cask "${cask_gui_tools[@]}"
brew install --cask "${media_gui_tools[@]}"
brew install --cask "${office_gui_tools[@]}"

