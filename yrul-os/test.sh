#!/bin/bash

# Yrul OS Project Startup Script

echo "YRUL OS - PROJECT STARTUP"
echo "=========================="

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    echo "Error: Not in yrul-os directory"
    echo "Please run from yrul-os/ directory"
    exit 1
fi

# Build the OS
echo "Building Yrul OS..."
if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
    echo "Build successful"
else
    echo "Build failed"
    echo "Run 'make' manually to see errors"
    exit 1
fi

echo ""
echo "SYSTEM FEATURES:"
echo "   • 32-bit x86 kernel with multiboot compliance"
echo "   • VGA text mode display (80x25)"
echo "   • PS/2 keyboard driver with interrupt handling"  
echo "   • Heap-based memory management (kmalloc/kfree)"
echo "   • Preemptive multitasking scheduler with timer IRQ"
echo "   • Command-line interface with interactive commands"
echo ""
echo "AVAILABLE COMMANDS:"
echo "   • help  - Show available commands"
echo "   • clear - Clear screen"
echo "   • info  - System information"
echo "   • test  - Test display"
echo "   • mem   - Memory statistics"
echo "   • ps    - Process information"
echo ""
echo "CONTROLS:"
echo "   • Type to input text"
echo "   • Enter to execute command"
echo "   • Backspace to correct"
echo "   • Ctrl+C to exit QEMU"
echo ""

read -p "Press Enter to start Yrul OS..."

echo "Starting Yrul OS..."
make run-stable

echo ""
echo "Yrul OS session completed"
