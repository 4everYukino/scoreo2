#!/bin/bash

set -euo pipefail

ETC_PREFIX="/etc/scoreo2"
OPT_PREFIX="/opt/scoreo2"
SERVICE_DIR="/lib/systemd/system"
SERVICE_NAME="scoreo2.service"

function help() {
    cat <<'EOF'
Usage: ./install.sh
    -h, --help: Print this help and exit
EOF
}

function pre_install() {
    echo "Checking status of '$SERVICE_NAME' ..."
    if systemctl is-active --quiet "$SERVICE_NAME"; then
        echo "The '$SERVICE_NAME' is running, stopping service ..."
        systemctl stop "$SERVICE_NAME"
    fi
}

while [[ $# -gt 0 ]]; do
    case "$1" in
    -h|--help)
        help
        exit 0
        ;;
    *)
        echo "Unknown option: $1" >&2
        exit 1
        ;;
    esac
done

# Verify root privileges
if [[ $EUID -ne 0 ]]; then
    echo "Error: This script must be run as root or via sudo." >&2
    exit 1
fi

pre_install

# 1. Install application binaries
echo "Installing application binaries to $OPT_PREFIX/bin ..."
install -d -m 755 "$OPT_PREFIX/bin"
install -m 755 opt/scoreo2/bin/starter "$OPT_PREFIX/bin/"

# 2. Install library files
echo "Installing library files to $OPT_PREFIX/lib ..."
install -d -m 755 "$OPT_PREFIX/lib"
for lib in opt/scoreo2/lib/*.so*; do
    echo "  Installing '$lib' -> $OPT_PREFIX/lib/ ..."
    install -m 644 "$lib" "$OPT_PREFIX/lib/"
done

# 3. Install conf files
echo "Installing config files to $ETC_PREFIX/conf ..."
install -d -m 755 "$ETC_PREFIX/conf"
for conf in etc/scoreo2/conf/*.conf; do
    echo "  Installing '$conf' -> $ETC_PREFIX/conf/ ..."
    install -m 644 "$conf" "$ETC_PREFIX/conf/"
done

# 4. Install systemd service
echo "Installing systemd service to $SERVICE_DIR"
install -d -m 755 "$SERVICE_DIR"
install -m 644 lib/systemd/system/scoreo2.service "$SERVICE_DIR/"

echo "Reloading systemd daemon ..."
systemctl daemon-reload

# 5. Completion message
echo "The scoreo2 installation is completed."
