#!/bin/bash

# Run all validation tests
cd /home/gabriel/Yrul3-0/yrul-os

echo "System Validation"
echo ""

# Run build test
echo "1. Build validation:"
if ./tests/build_test.sh | tail -n 1 | grep -q "CLEAN\|WARNINGS"; then
    echo "   Build: PASSED"
else
    echo "   Build: FAILED"
    exit 1
fi

# Run function validation
echo "2. Function validation:"
if ./tests/function_validation.sh | tail -n 1 | grep -q "OPERATIONAL"; then
    echo "   Functions: PASSED"
else
    echo "   Functions: FAILED"
    exit 1
fi

# Run memory validation
echo "3. Memory validation:"
if ./tests/memory_validation.sh | tail -n 1 | grep -q "OPERATIONAL"; then
    echo "   Memory: PASSED"
else
    echo "   Memory: FAILED"
    exit 1
fi

echo ""
echo "SYSTEM STATUS: ALL TESTS PASSED"

echo ""
echo "SYSTEM STATUS: ALL TESTS PASSED"
