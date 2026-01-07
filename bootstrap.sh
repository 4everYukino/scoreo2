#!/bin/bash
set -euo pipefail

# -----------------------------------------------------------------------------
# bootstrap.sh - build, (optionally) test, install and package project
# -----------------------------------------------------------------------------

CLEAN=false

CC="gcc"
CXX="g++"

PROJECT_NAME="scoreo2"
BUILD_DIR="$(pwd)"
BUILD_SUBDIR="${BUILD_DIR}/build"
STAGING_DIR="${BUILD_DIR}/build/_package"
INSTALL_PREFIX="/opt/${PROJECT_NAME}"

BUILD_TYPE="Release"
ENABLE_TESTS="${ENABLE_TESTS:-OFF}"
VERBOSE="OFF"

function log() {
  printf '%s: %s\n' "$(date +'%Y-%m-%d %H:%M:%S')" "$*"
}

function err() {
  log "ERROR: $*" >&2
}

function usage() {
  cat <<EOF
Usage: $0 [options]

Options:
  -h, --help               Show this help message

  -c, --clean              Clean build directory before building
  -d, --debug              Build debug version
  -t, --tests              Enable building and running unit tests
  -v, --verbose            Enable verbose output

Environment:
  ENABLE_TESTS=ON          Alternative way to enable tests
  VERBOSE=ON               Alternative way to enable verbose logs

Examples:
  $0
  $0 -tests
  ENABLE_TESTS=ON VERBOSE=ON $0
EOF
}

function parse_args() {
  while [[ $# -gt 0 ]]; do
    case "$1" in
      -h|--help)
        usage
        exit 0
        ;;
      -c|--clean)
        CLEAN=true
        shift
        ;;
      -d|--debug)
        BUILD_TYPE="Debug"
        shift
        ;;
      -t|--tests)
        ENABLE_TESTS="ON"
        shift
        ;;
      -v|--verbose)
        VERBOSE="ON"
        shift
        ;;
      *)
        err "Unknown argument: $1"
        usage
        exit 2
        ;;
    esac
  done
}

function prepare_compiler() {
  log "Checking compiler ..."

  if command -v "gcc-13" >/dev/null 2>&1; then
    CC="gcc-13"
  fi

  if command -v "g++-13" >/dev/null 2>&1; then
    CXX="g++-13"
  fi

  log "Using C Compiler ${CC}, CXX Compiler ${CXX} ..."
}

function prepare_dirs() {
  log "Preparing build and staging directories ..."

  if $CLEAN; then
    rm -rf "${BUILD_SUBDIR}"
  fi

  rm -f "${PROJECT_NAME}.tar.gz"

  mkdir -p "${BUILD_SUBDIR}"
  mkdir -p "${STAGING_DIR}"
}

function run_cmake() {
  log "Configuring project with CMake (ENABLE_TESTS=${ENABLE_TESTS}, VERBOSE=${VERBOSE}) ..."
  cd "${BUILD_SUBDIR}"

  cmake -DCMAKE_C_COMPILER=${CC} \
        -DCMAKE_CXX_COMPILER=${CXX} \
        -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DENABLE_TESTS="${ENABLE_TESTS}" \
        -DCMAKE_VERBOSE_MAKEFILE="${VERBOSE}" \
        ..
}

function build_project() {
  log "Building project ..."
  make -j
}

function run_tests() {
  log "Running unit tests ..."

  cd "${BUILD_SUBDIR}"

  if [[ -x "${BUILD_SUBDIR}/tests/run_tests" ]]; then
    if [[ "${VERBOSE}" == "ON" ]]; then
      "${BUILD_SUBDIR}/tests/run_tests" --gtest_color=yes
    else
      "${BUILD_SUBDIR}/tests/run_tests"
    fi

    return $?
  fi

  err "No test runner found."
  return 2
}

function install_project() {
  log "Installing project to staging dir (${STAGING_DIR}) ..."

  opts="DESTDIR=${STAGING_DIR} install"
  if [[ "${VERBOSE}" == "ON" ]]; then
    opts="$opts VERBOSE=1"
  fi

  make $opts > /dev/null

  install -m 755 "${BUILD_DIR}/install.sh" "${STAGING_DIR}"
}

function package_project() {
  log "Packaging project into ${PROJECT_NAME}.tar.gz ..."
  cd "${STAGING_DIR}"

  tar --exclude="opt/${PROJECT_NAME}/include" \
      --exclude="opt/${PROJECT_NAME}/lib/cmake" \
      --exclude="opt/${PROJECT_NAME}/lib/pkgconfig" \
      -czf "${BUILD_DIR}/${PROJECT_NAME}.tar.gz" etc lib opt "install.sh"

  log "Package '${BUILD_DIR}/${PROJECT_NAME}.tar.gz' created."
}

function main() {
  parse_args "$@"

  prepare_compiler

  prepare_dirs

  run_cmake

  build_project

  if [[ "${ENABLE_TESTS}" == "ON" ]]; then
    run_tests
  fi

  install_project

  package_project

  log "Build '${PROJECT_NAME}.tar.gz' finished."
}

main "$@"
