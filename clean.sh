#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd "$ROOT_DIR"

rm -rf Binaries Objects Reports
rm -f Makefile ./*.make ./**/*.make ./**/Makefile
rm -f *.workspace ./*.workspace ./**/*.workspace
rm -f *.sln ./*.sln ./**.*.sln
rm -f *.vcxproj* ./*.vcxproj* ./**.*.vcxproj*
rm -f *.csproj ./*.csproj ./**.*.csproj

printf 'Cleaned generated build files and directories.\n'
