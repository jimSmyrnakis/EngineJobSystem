#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd "$ROOT_DIR"

rm -rf ./**/bin ./**/obj ./**/lib
rm -f Makefile ./**/*.make
rm -f ./**/*.make
rm -f ./**/*.workspace
rm -f ./**/*.sln
rm -f ./**/*.vcxproj
rm -f ./**/*.csproj

printf 'Cleaned generated build files and directories.\n'
