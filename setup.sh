#!/bin/bash

# Yrul OS Setup and Build Script
# Works on any Unix-like system with proper tools installed

set -e

echo "Yrul OS - Setup and Build"
echo "========================="

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_ROOT="$SCRIPT_DIR"

echo "Project directory: $PROJECT_ROOT"

if [ ! -f "$PROJECT_ROOT/yrul-os/Makefile" ]; then
    echo "Error: Cannot find yrul-os/Makefile"
    echo "Make sure you're running this from the project root"
    exit 1
fi

cd "$PROJECT_ROOT/yrul-os"

echo ""
echo "Checking required tools..."

check_tool() {
    if command -v "$1" > /dev/null 2>&1; then
        echo "$1 found"
        return 0
    else
        echo "$1 not found"
        return 1
    fi
}

tools_ok=true

if ! check_tool "gcc"; then
    tools_ok=false
fi

if ! check_tool "ld"; then
    tools_ok=false
fi

if ! check_tool "as"; then
    tools_ok=false
fi

if ! check_tool "make"; then
    tools_ok=false
fi

if ! check_tool "qemu-system-i386"; then
    echo "qemu-system-i386 not found"
    echo "    Note: QEMU is optional for building, required only for running"
fi

if ! $tools_ok; then
    echo ""
    echo "Missing required tools. Please install:"
    echo "Ubuntu/Debian: sudo apt-get install gcc-multilib nasm make qemu-system-x86"
    echo "Arch Linux: sudo pacman -S gcc-multilib nasm make qemu-system-i386"
    echo "Fedora/RHEL: sudo dnf install gcc gcc-multilib nasm make qemu-system-x86"
    exit 1
fi

echo ""
echo "All required tools found!"

echo ""
echo "Testing 32-bit compilation support..."
if echo 'int main() { return 0; }' | gcc -m32 -x c - -o /tmp/test32 > /dev/null 2>&1; then
    echo "32-bit compilation supported"
    rm -f /tmp/test32
else
    echo "32-bit compilation not supported"
    echo "    Install 32-bit development libraries:"
    echo "    Ubuntu/Debian: sudo apt-get install gcc-multilib libc6-dev-i386"
    exit 1
fi

echo ""
echo "Building Yrul OS..."
if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
    echo "Build successful"
else
    echo "Build failed"
    echo "    Run 'make' to see detailed error messages"
    exit 1
fi

echo ""
echo "Running validation tests..."
if ./tests/system_validation.sh > /dev/null 2>&1; then
    echo "All tests passed"
else
    echo "Some tests failed"
    echo "    Run './tests/system_validation.sh' to see details"
    exit 1
fi

echo ""
echo "Setup completed successfully!"
echo ""
echo "Available commands:"
echo "  ./test.sh                    - Quick start (interactive)"
echo "  make run                     - Run OS in QEMU"
echo "  make run-stable              - Run with stable QEMU settings"
echo "  ./tests/system_validation.sh - Run all validation tests"
echo ""
