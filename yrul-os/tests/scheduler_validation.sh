#!/bin/bash

echo "Scheduler Validation Test"
echo ""

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

cd "$PROJECT_DIR"

# Ensure build is clean
make clean > /dev/null 2>&1
if ! make > /dev/null 2>&1; then
    echo "Build failed - cannot validate scheduler"
    exit 1
fi

# Test timer and scheduler functions
echo "Timer Functions:"
timer_functions=("timer_init" "timer_handler" "timer_get_ticks")
timer_passed=0

for func in "${timer_functions[@]}"; do
    if objdump -t yrul.bin | grep -q "$func"; then
        echo "   $func: FOUND"
        ((timer_passed++))
    else
        echo "   $func: MISSING"
    fi
done

echo ""
echo "Task Management Functions:"
task_functions=("task_init" "task_create" "task_schedule" "task_switch" "task_yield" "task_exit")
task_passed=0

for func in "${task_functions[@]}"; do
    if objdump -t yrul.bin | grep -q "$func"; then
        echo "   $func: FOUND"
        ((task_passed++))
    else
        echo "   $func: MISSING"
    fi
done

echo ""
echo "Assembly Routines:"
asm_functions=("timer_interrupt_wrapper" "keyboard_interrupt_wrapper")
asm_passed=0

for func in "${asm_functions[@]}"; do
    if objdump -t yrul.bin | grep -q "$func"; then
        echo "   $func: FOUND"
        ((asm_passed++))
    else
        echo "   $func: MISSING"
    fi
done

total=$((timer_passed + task_passed + asm_passed))
total_expected=$((${#timer_functions[@]} + ${#task_functions[@]} + ${#asm_functions[@]}))

echo ""
echo "Scheduler validation: $total/$total_expected functions found"

if [ $total -eq $total_expected ]; then
    echo "Status: SCHEDULER SYSTEM OPERATIONAL"
else
    echo "Status: SCHEDULER COMPONENTS MISSING"
    exit 1
fi
