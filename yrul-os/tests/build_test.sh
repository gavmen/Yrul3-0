#!/bin/bash

echo "Build Test"
echo ""

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Build test
cd "$PROJECT_DIR"
make clean > /dev/null 2>&1

if make > build.log 2>&1; then
    if [ -s build.log ] && grep -q "warning\|error" build.log; then
        echo "Build completed with warnings:"
        grep -E "(warning|error)" build.log | head -5
        echo "Build status: WARNINGS"
    else
        echo "Build successful - no warnings or errors"
        echo "Build status: CLEAN"
    fi
else
    echo "Build failed:"
    grep -E "(error|Error)" build.log | head -5
    echo "Build status: FAILED"
    exit 1
fi

rm -f build.log
