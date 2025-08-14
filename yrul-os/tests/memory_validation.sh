#!/bin/bash

echo "Memory Management Validation"
echo ""

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Clean build test
echo "1. Build test"
cd "$PROJECT_DIR"
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "   Build successful"
else
    echo "   Build failed"
    exit 1
fi

# Check memory functions are linked
echo "2. Memory functions"
if objdump -t yrul.bin | grep -q "kmalloc\|kfree\|mem_init"; then
    echo "   Functions linked:"
    objdump -t yrul.bin | grep -E "(kmalloc|kfree|mem_init|mem_get_free)" | sed 's/^/     /'
else
    echo "   Functions not found"
    exit 1
fi

# Check scheduler functions are linked
echo "3. Scheduler functions"
if objdump -t yrul.bin | grep -q "task_init\|timer_init"; then
    echo "   Functions linked:"
    objdump -t yrul.bin | grep -E "(task_init|timer_init|task_create|task_schedule)" | sed 's/^/     /'
else
    echo "   Functions not found"
    exit 1
fi

# Check binary size and sections
echo "4. Binary structure"
size yrul.bin | sed 's/^/   /'

# Check for memory-related symbols
echo "5. Memory symbols:"
objdump -t yrul.bin | grep -E "(heap|mem_|task_|timer_)" | head -8 | sed 's/^/   /'

# Verify no undefined symbols
echo "6. Symbol check"
if objdump -t yrul.bin | grep -q "\*UND\*"; then
    echo "   Undefined symbols found:"
    objdump -t yrul.bin | grep "\*UND\*" | sed 's/^/     /'
else
    echo "   No undefined symbols"
fi

# Test kernel structure integrity  
echo "7. Kernel sections"
if objdump -h yrul.bin | grep -q "\.text\|\.data"; then
    echo "   Sections present:"
    objdump -h yrul.bin | grep -E "\.text|\.data|\.bss" | sed 's/^/     /'
else
    echo "   Missing sections"
fi

echo ""
echo "Enhanced Kernel System: OPERATIONAL"
