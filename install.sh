#!/bin/bash

set -euo pipefail

PREFIX="/opt/scoreo2"
FORCE=0
SERVICE_NAME="scoreo2.service"

function help() {
    cat <<'EOF'
Usage: ./install.sh [-p PREFIX] [-f]
    -h, --help: Print this help and exit
    -p, --prefix: Specify the install prefix [default: "/opt/scoreo2"]
    -f, --force: Force overwrite if the target exists
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
    -p|--prefix)
        PREFIX="$2"
        shift 2
        ;;
    -f|--force)
        FORCE=1
        shift
        ;;
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

# Force mode: remove existing directory if requested
if [[ $FORCE -eq 1 && -d "$PREFIX" ]]; then
    echo "Force mode: removing existing directory $PREFIX"
    rm -rf "$PREFIX"
fi

# 1. Install application binaries
echo "Creating directory $PREFIX/bin"
install -d -m 755 "$PREFIX/bin"
install -m 755 opt/scoreo2/bin/starter "$PREFIX/bin/"

# 2. Install library files
echo "Creating directory $PREFIX/lib"
install -d -m 755 "$PREFIX/lib"
for lib in opt/scoreo2/lib/*.so*; do
    echo "Installing library $lib -> $PREFIX/lib/"
    install -m 644 "$lib" "$PREFIX/lib/"
done

# 3. Install systemd service
SERVICE_DIR="/lib/systemd/system"
echo "Installing systemd service to $SERVICE_DIR"
install -d -m 755 "$SERVICE_DIR"
install -m 644 lib/systemd/system/scoreo2.service "$SERVICE_DIR/"

echo "Reloading systemd daemon"
systemctl daemon-reload

# 4. Completion message
echo "The scoreo2 installation is completed."
