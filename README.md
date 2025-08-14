# Yrul OS

**Yrul OS** is a simple, educational operating system built from scratch, focusing on low-level hardware interaction, interrupt handling, keyboard input management, and memory allocation.

---

## Current Project Status

### Core Features
- Create a minimal OS that boots on x86 architecture
- Implement proper interrupt handling (PIC and IDT configuration)
- Develop a working keyboard driver with PS/2 controller support
- Provide a simple command-line interface
- Heap-based dynamic memory management system

---

## Architecture

### Components

#### Boot System
- **Multiboot compliant bootloader**
- **Supports both legacy and modern QEMU**
- **32-bit x86 architecture**

#### Interrupt Management
- **PIC (Programmable Interrupt Controller) configuration**
- **IDT (Interrupt Descriptor Table) setup**
- **Hardware interrupt handling**

#### Memory Management
- **Heap-based allocation system**
- **kmalloc() and kfree() functions**
- **Memory statistics and diagnostics**
- **1MB heap space at 0x200000**

#### Keyboard System
- **PS/2 keyboard controller driver**
- **Scancode to ASCII conversion**
- **Interrupt-based input processing**
- **Command line interface with interactive commands**

#### Display System
- **VGA text mode (80x25)**
- **Direct video memory manipulation**
- **Color-coded output**

---

## Building and Running

### Prerequisites

#### Required Tools
```bash
# Ubuntu/Debian
sudo apt-get install gcc-multilib nasm qemu-system-x86

# Arch Linux
sudo pacman -S gcc-multilib nasm qemu-system-i386

# Fedora/RHEL
sudo dnf install gcc gcc-multilib nasm qemu-system-x86
```

### Building

```bash
# Clone the repository
git clone https://github.com/gavmen/Yrul3-0
cd Yrul3-0

# Quick setup (checks dependencies and builds)
./setup.sh

# Manual build process
cd yrul-os
make clean && make
```

### Running

#### Quick Start
```bash
# From project root
cd yrul-os
./test.sh
```

#### Standard Execution
```bash
make run
```

#### Testing and Validation
```bash
# Run all validation tests
./tests/system_validation.sh

# Run individual tests
./tests/build_test.sh
./tests/function_validation.sh
./tests/memory_validation.sh
```

---

## Usage

### Available Commands
- **help** - Show available commands
- **clear** - Clear screen
- **info** - System information
- **test** - Test display
- **mem** - Memory statistics

### Controls
- **Type** to input text
- **Enter** to execute command
- **Backspace** to correct errors
- **Ctrl+C** to exit QEMU

---

## Technical Details

### File Structure

```
yrul-os/
├── boot/
│   ├── boot.s          # Multiboot bootloader
│   ├── grub.cfg        # GRUB configuration
│   └── linker.ld       # Linker script
├── src/
│   ├── kernel.c        # Main kernel
│   ├── interrupts.c/h  # Interrupt management
│   ├── keyboard.c/h    # Keyboard driver
│   ├── memory.c/h      # Memory management
│   ├── io.c/h         # I/O operations
│   ├── string.c/h      # String utilities
│   └── types.h        # Type definitions
├── tests/
│   ├── build_test.sh          # Build validation
│   ├── function_validation.sh # Function verification
│   ├── memory_validation.sh   # Memory system test
│   └── system_validation.sh   # Complete validation
├── Makefile           # Build system
└── test.sh           # Project startup script
```

### Memory Layout
- **Kernel loaded at**: 1MB (0x100000)
- **Heap space**: 1MB starting at 0x200000
- **Video memory**: 0xB8000 (VGA text mode)
- **Stack**: High memory, grows downward

### Interrupt Configuration
- **PIC remapped**: Master (0x20-0x27), Slave (0x28-0x2F)
- **Keyboard IRQ**: IRQ1 (INT 0x21)
- **IDT size**: 256 entries

---

## Features

### Current Features
- Multiboot-compliant bootloader
- Hardware interrupt handling
- PS/2 keyboard driver
- VGA text mode display
- Command line interface
- Heap-based memory management
- Interactive command system
- Comprehensive validation test suite

### System Components
- Kernel initialization and main loop
- Interrupt descriptor table (IDT) setup
- Programmable interrupt controller (PIC) configuration
- Memory allocator with kmalloc/kfree functions
- String manipulation utilities
- I/O port operations

---

## Troubleshooting

### Common Issues

#### Build Errors
Ensure all required tools are installed and you have proper 32-bit compilation support.

#### System Hangs
Ensure QEMU version compatibility. Try different QEMU parameters.

#### Keyboard Not Working
The system uses interrupt-based keyboard input. Check hardware compatibility.

---

## License

This project is released under the MIT License. See LICENSE file for details.

---

## Project Status

**Status**: Active Development  
**Last Updated**: August 2025
