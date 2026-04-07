#!/bin/bash

set -euo pipefail

ETC_PREFIX="/etc/scoreo2"
OPT_PREFIX="/opt/scoreo2"
SERVICE_DIR="/lib/systemd/system"
SERVICE_NAME="scoreo2.service"

PACKAGE_PATH=""
TEMP_DIR=""

function help() {
    cat <<'EOF'
Usage: ./unpack_install.sh <package.tar.gz>
    -h, --help: Print this help and exit
EOF
}

function cleanup() {
    echo "[DEBUG] Cleanup '$TEMP_DIR' ..."
    if [[ -n "$TEMP_DIR" && -d "$TEMP_DIR" ]]; then
        rm -rf "$TEMP_DIR"
    fi
}

function pre_install() {
    echo "[DEBUG] Checking status of '$SERVICE_NAME' ..."
    if systemctl is-active --quiet "$SERVICE_NAME"; then
        echo "[DEBUG] The '$SERVICE_NAME' is running, stopping service ..."
        systemctl stop "$SERVICE_NAME"
    fi
}

function post_install() {
    echo "[DEBUG] Starting '$SERVICE_NAME' ..."
    if ! systemctl restart "$SERVICE_NAME"; then
        echo "[ERROR] Failed to start '$SERVICE_NAME', check logs for more details." >&2
    fi

    echo "[DEBUG] Enabling '$SERVICE_NAME' to start on boot ..."
    if ! systemctl enable "$SERVICE_NAME"; then
        echo "[WARNING] Failed to enable '$SERVICE_NAME', check logs for more details." >&2
    fi
}

function parse_args() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
        -h|--help)
            help
            exit 0
            ;;
        -*)
            echo "[ERROR] Unknown option: $1" >&2
            exit 1
            ;;
        *)
            if [[ -n "$PACKAGE_PATH" ]]; then
                echo "[ERROR] Only one package path is allowed." >&2
                exit 1
            fi

            PACKAGE_PATH="$1"
            shift
            ;;
        esac
    done

    if [[ -z "$PACKAGE_PATH" ]]; then
        echo "[ERROR] Missing package path." >&2
        help
        exit 1
    fi
}

function validate_package() {
    if [[ ! -f "$PACKAGE_PATH" ]]; then
        echo "[ERROR] Package '$PACKAGE_PATH' does not exist." >&2
        exit 1
    fi
}

function prepare_temp_dir() {
    TEMP_DIR="$(mktemp -d "scoreo2.XXXXXX")"
    trap cleanup EXIT
}

function unpack_package() {
    echo "[DEBUG] Unpacking '$PACKAGE_PATH' to '$TEMP_DIR' ..."
    tar -xzf "$PACKAGE_PATH" -C "$TEMP_DIR"
}

function validate_layout() {
    local package_root="$1"

    if [[ ! -d "$package_root/opt/scoreo2/bin" ]]; then
        echo "[ERROR] Missing directory '$package_root/opt/scoreo2/bin'." >&2
        exit 1
    fi

    if [[ ! -d "$package_root/opt/scoreo2/lib" ]]; then
        echo "[ERROR] Missing directory '$package_root/opt/scoreo2/lib'." >&2
        exit 1
    fi

    if [[ ! -d "$package_root/etc/scoreo2/conf" ]]; then
        echo "[ERROR] Missing directory '$package_root/etc/scoreo2/conf'." >&2
        exit 1
    fi

    if [[ ! -f "$package_root/lib/systemd/system/$SERVICE_NAME" ]]; then
        echo "[ERROR] Missing file '$package_root/lib/systemd/system/$SERVICE_NAME'." >&2
        exit 1
    fi
}

function install_binaries() {
    local package_root="$1"

    echo "[DEBUG] Installing binaries to $OPT_PREFIX/bin ..."
    install -d -m 755 "$OPT_PREFIX/bin"
    for bin in "$package_root"/opt/scoreo2/bin/*; do
        echo "[DEBUG]   Installing '$bin' -> $OPT_PREFIX/bin/ ..."
        install -m 755 "$bin" "$OPT_PREFIX/bin/"
    done
}

function install_libraries() {
    local package_root="$1"

    echo "[DEBUG] Installing libraries to $OPT_PREFIX/lib ..."
    install -d -m 755 "$OPT_PREFIX/lib"
    for lib in "$package_root"/opt/scoreo2/lib/*.so*; do
        echo "[DEBUG]   Installing '$lib' -> $OPT_PREFIX/lib/ ..."
        install -m 644 "$lib" "$OPT_PREFIX/lib/"
    done
}

function install_configs() {
    local package_root="$1"

    echo "[DEBUG] Installing configs to $ETC_PREFIX/conf ..."
    install -d -m 755 "$ETC_PREFIX/conf"
    for conf in "$package_root"/etc/scoreo2/conf/*.conf; do
        echo "[DEBUG]   Installing '$conf' -> $ETC_PREFIX/conf/ ..."
        install -m 644 "$conf" "$ETC_PREFIX/conf/"
    done
}

function install_service() {
    local package_root="$1"

    echo "[DEBUG] Installing systemd service to $SERVICE_DIR"
    install -d -m 755 "$SERVICE_DIR"
    install -m 644 "$package_root/lib/systemd/system/$SERVICE_NAME" "$SERVICE_DIR/"
}

function install_package() {
    local package_root="$1"

    pre_install

    install_binaries "$package_root"
    install_libraries "$package_root"
    install_configs "$package_root"
    install_service "$package_root"

    echo "[DEBUG] Reloading systemd daemon ..."
    systemctl daemon-reload

    echo "[DEBUG] The scoreo2 installation is completed."

    post_install
}

parse_args "$@"

# Verify root privileges
if [[ $EUID -ne 0 ]]; then
    echo "[ERROR] This script must be run as root or via sudo." >&2
    exit 1
fi

validate_package
prepare_temp_dir
unpack_package
validate_layout "$TEMP_DIR"
install_package "$TEMP_DIR"
