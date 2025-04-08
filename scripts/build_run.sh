#!/bin/bash
set -e

BUILD_TYPE="Debug"
TARGET_WASM=false

print_usage() {
    echo "Usage: $0 [--build-type <Debug|Release>] [--target-wasm]"
    echo "  --build-type: Set build type (Debug or Release). Default: Debug"
    echo "  --target-wasm: Build for WebAssembly using Emscripten"
    exit 1
}

while [[ $# -gt 0 ]]; do
    case $1 in
        --build-type)
            if [[ "$2" != "Debug" && "$2" != "Release" ]]; then
                echo "Error: Build type must be either Debug or Release"
                print_usage
            fi
            BUILD_TYPE="$2"
            shift 2
            ;;
        --target-wasm)
            TARGET_WASM=true
            shift
            ;;
        *)
            echo "Unknown argument: $1"
            print_usage
            ;;
    esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKSPACE_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

cd "$WORKSPACE_DIR"

if [ "$TARGET_WASM" = true ]; then
    if ! command -v emcc >/dev/null 2>&1; then
        echo "Error: Emscripten (emcc) is required for WebAssembly builds but was not found"
        exit 1
    fi
fi

if [ "$TARGET_WASM" = true ]; then
    echo "Creating web build directory..."
    mkdir -p build/wasm
    mkdir -p bin/wasm

    echo "Configuring CMake for Emscripten..."
    emcmake cmake -B build/wasm \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DPLATFORM=Wasm \
        -DCMAKE_EXECUTABLE_SUFFIX=".html"

    echo "Building project for web..."
    cmake --build build/wasm --clean-first

    if command -v node >/dev/null 2>&1; then
        cd bin/wasm
        echo "Running web server..."
        node serve.js
    else
        echo "Note: Node.js not found. Skipping web server startup."
        echo "To run the web server manually, navigate to bin/wasm and run: node serve.js"
    fi
else
    echo "Creating native build directory..."
    mkdir -p build

    echo "Configuring CMake for native build..."
    cmake -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

    echo "Building project..."
    cmake --build build --clean-first

    echo "Running native executable..."
    ./bin/raylib_test
fi

cd "$WORKSPACE_DIR"
exit 0
