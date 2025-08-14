#!/bin/bash

echo "Build Test"
echo ""

# Build test
cd /home/gabriel/Yrul3-0/yrul-os
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
