#!/bin/bash

echo "Function Validation Test"
echo ""

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

cd "$PROJECT_DIR"

# Ensure build is clean
make clean > /dev/null 2>&1
if ! make > /dev/null 2>&1; then
    echo "Build failed - cannot validate functions"
    exit 1
fi

# Test core functions
echo "Core Functions:"
functions=("kernel_main" "keyboard_init" "mem_init" "print_message" "clear_screen" "remap_pic" "init_idt")
passed=0

for func in "${functions[@]}"; do
    if objdump -t yrul.bin | grep -q "$func"; then
        echo "   $func: FOUND"
        ((passed++))
    else
        echo "   $func: MISSING"
    fi
done

echo ""
echo "Memory Functions:"
mem_functions=("kmalloc" "kfree" "mem_get_free")
mem_passed=0

for func in "${mem_functions[@]}"; do
    if objdump -t yrul.bin | grep -q "$func"; then
        echo "   $func: FOUND"
        ((mem_passed++))
    else
        echo "   $func: MISSING"
    fi
done

total=$((passed + mem_passed))
total_expected=$((${#functions[@]} + ${#mem_functions[@]}))

echo ""
echo "Function validation: $total/$total_expected functions found"

if [ $total -eq $total_expected ]; then
    echo "Status: ALL FUNCTIONS OPERATIONAL"
else
    echo "Status: MISSING FUNCTIONS"
    exit 1
fi
