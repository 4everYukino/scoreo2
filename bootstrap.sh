#!/bin/bash
set -euo pipefail

PROJECT_NAME="scoreo2"
BUILD_DIR="$(pwd)"
BUILD_SUBDIR="${BUILD_DIR}/build"
STAGING_DIR="${BUILD_DIR}/build/_package"
INSTALL_PREFIX="/opt/${PROJECT_NAME}"

# rm -rf "${BUILD_SUBDIR}"
rm -f "${PROJECT_NAME}.tar.gz"

mkdir -p "${BUILD_SUBDIR}"
mkdir -p "${STAGING_DIR}"

SECONDS=0

cd "${BUILD_SUBDIR}"
cmake -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=ON ..
make -j"$(nproc)"
make DESTDIR="${STAGING_DIR}" install

install -m 755 "${BUILD_DIR}/install.sh" "${STAGING_DIR}"

cd "${STAGING_DIR}"
tar --exclude="opt/${PROJECT_NAME}/include" \
    --exclude="opt/${PROJECT_NAME}/lib/cmake" \
    --exclude="opt/${PROJECT_NAME}/lib/pkgconfig" \
    -czf "${BUILD_DIR}/${PROJECT_NAME}.tar.gz" lib opt "install.sh"

duration=$SECONDS

echo "Build '${PROJECT_NAME}.tar.gz' finished, cost ${duration} seconds"
