#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Run all validation tests
cd "$PROJECT_DIR"

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
