#!/usr/bin/env bash
set -euo pipefail

APP_NAME="scoreo2"
BUILD_DIR="$(pwd)"
BUILD_SUBDIR="${BUILD_DIR}/build"
STAGING_DIR="${BUILD_DIR}/build/_package"
INSTALL_PREFIX="/opt/${APP_NAME}"

rm -rf "${BUILD_SUBDIR}" "${APP_NAME}.tar.gz"

mkdir -p "${BUILD_SUBDIR}"
mkdir -p "${STAGING_DIR}"

SECONDS=0

cd "${BUILD_SUBDIR}"
cmake -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" ..
make -j"$(nproc)"
make DESTDIR="${STAGING_DIR}" install

cd "${STAGING_DIR}"
tar -czf "${BUILD_DIR}/${APP_NAME}.tar.gz" opt

duration=$SECONDS

echo "Build '${APP_NAME}.tar.gz' finished, cost ${duration} seconds"
