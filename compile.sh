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
LIB_EXT_DIR="${ROOT_DIR}/lib_ext"

LVGL_DIR="${LIB_EXT_DIR}/lvgl"
GLAZE_DIR="${LIB_EXT_DIR}/glaze"
LIBARTNET_DIR="${LIB_EXT_DIR}/libartnet"

# Ordner anlegen
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

if [ ! -d "$LIB_EXT_DIR" ]; then
    mkdir -p "$LIB_EXT_DIR"
fi

# externe Bibliotheken holen
git clone --depth 1 --single-branch --branch v9.5.0 https://github.com/lvgl/lvgl.git ${LVGL_DIR} || true
git clone --depth 1 --single-branch --branch v7.7.1 https://github.com/stephenberry/glaze.git ${GLAZE_DIR} || true
git clone --depth 1 --single-branch https://github.com/OpenLightingProject/libartnet.git ${LIBARTNET_DIR} || true

make BUILD_DIR=${BUILD_DIR} -j$(nproc)