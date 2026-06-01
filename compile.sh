#!/usr/bin/env bash
set -euo pipefail

export PATH=/opt/cross/bin:$PATH

# X32/M32
CPU=arm926ej-s
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabi-

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

BUILD_DIR="${ROOT_DIR}/build"
SRC_DIR="${ROOT_DIR}/src"
LVGL_DIR="${ROOT_DIR}/lib/lv_port_linux"
GLAZE_PATH="${ROOT_DIR}/lib/glaze/include"

git submodule update --init --recursive

cd "$SRC_DIR"

make -j14

cd "$ROOT_DIR"