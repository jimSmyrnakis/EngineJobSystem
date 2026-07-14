#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

if ! command -v premake5 >/dev/null 2>&1; then
    echo "premake5 is required but was not found in PATH." >&2
    exit 1
fi

run_coverage()
{
    local configs=("Debug" "Release" )
    case "$PLATFORM" in
        x64)
            PLAT="x64"
            ;;
        x86)
            PLAT="x86"
            ;;
        arm32)
            PLAT="ARM32"
            ;;
        arm64)
            PLAT="ARM64"
            ;;
        *)
            PLAT="$PLATFORM"
            ;;
    esac
    for cfg in "${configs[@]}"; do

        echo "===================================="
        echo "Coverage: ${PLATFORM} ${cfg}"
        echo "===================================="

        #make config="${cfg}_${PLATFORM}"

        find "./Binaries/Linux" \
            -path "*${PLAT}/${cfg}/*" \
            -name "TestSDK"\
            -type f \
            -executable \
            | while read -r exe; do

                echo "Running: $exe"
                "$exe"

          done


        mkdir -p "Reports/coverage/Linux/${PLAT}/${cfg}"


        gcovr \
            --root . \
            --object-directory "Objects/Linux/${PLAT}/${cfg}" \
            --html \
            --html-details \
            -o "Reports/coverage/Linux/${PLAT}/${cfg}/index.html"

        gcovr \
        --root . \
        --object-directory "Objects/Linux/${PLAT}/${cfg}" 
       
    done
}

run_performance()
{
    configs=("Release" "Dist")
    case "$PLATFORM" in
        x64)
            PLAT="x64"
            ;;
        x86)
            PLAT="x86"
            ;;
        arm32)
            PLAT="ARM32"
            ;;
        arm64)
            PLAT="ARM64"
            ;;
        *)
            PLAT="$PLATFORM"
            ;;
    esac

    for cfg in "${configs[@]}"; do

        echo "=============================="
        echo "Performance ${PLAT} ${cfg}"
        echo "=============================="


        mkdir -p \
        "Reports/perf/Linux/${PLAT}/${cfg}"


        find "./Binaries/Linux/${PLAT}/${cfg}" \
        -type f \
        -executable |
        while read -r exe
        do

            name=$(basename "$exe")


            "$exe" \
            --benchmark_format=json \
            "--benchmark_out=Reports/perf/Linux/${PLAT}/${cfg}/${name}.json" 

            #python3 perf.py "./Reports/perf/Linux/${PLAT}/${cfg}/${name}.json"
        done

    done
}

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
call_premake(){
    
    premake5 gmake2
    
}

PLATFORM="$(detect_platform)"
ACTION="${1:-build}"
MODE="${2:-mode}"


case "$ACTION" in
    configure)
        call_premake
        ;;
    build)
        call_premake
        make config="debug_${PLATFORM}"
        make config="release_${PLATFORM}"
        make config="dist_${PLATFORM}"
        ;;
    clean)
        make clean
        ;;
    tests)
        find "./Binaries/Linux" -type f -name "TestSDK" | while read -r exe; do
            echo "Running $exe"
            "$exe"
        done
        ;;
    
    coverage)
    run_coverage
    ;;
    performance)

        

        run_performance

    ;;
    *)
        echo "Usage: ./build.sh [configure^|build^|clean^|setup^|tests^|coverage^|performance]" >&2
        exit 2
        ;;
esac


