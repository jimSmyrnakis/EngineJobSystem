#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

if ! command -v premake5 >/dev/null 2>&1; then
    echo "premake5 is required but was not found in PATH." >&2
    exit 1
fi

detect_platform() {
    local arch
    arch="$(uname -m 2>/dev/null || echo unknown)"
    case "$arch" in
        x86_64|amd64|x64) echo "x64" ;;
        i386|i486|i586|i686|x86) echo "x86" ;;
        armv6l|armv7l|armv7|armhf|arm) echo "arm32" ;;
        aarch64|arm64|armv8l|armv8) echo "arm64" ;;
        *) echo "x64" ;;
    esac
}

PLATFORM="$(detect_platform)"
ACTION="${1:-build}"
case "$ACTION" in
    configure)
        premake5 gmake2
        ;;
    build)
        premake5 gmake2
        make config="debug_${PLATFORM}"
        make config="release_${PLATFORM}"
        make config="dist_${PLATFORM}"
        ;;
    clean)
        make clean
        ;;
    setup)
        premake5 gmake2
        make config="debug_${PLATFORM}"
        make config="release_${PLATFORM}"
        make config="dist_${PLATFORM}"
        ;;
    tests)
        ;;
    coverage)
        ;;
    performance)
        ;;
    *)
        echo "Usage: ./build.sh [configure|build|clean|setup|tests|coverage|performance]" >&2
        exit 2
        ;;
esac


