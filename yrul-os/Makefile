# Makefile for Yrul OS - Clean and optimized version

CC = gcc
LD = ld
AS = as

# Compiler flags for kernel development
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 \
         -nostdlib -nostdinc -fno-builtin -fno-stack-protector

# Assembler flags
ASFLAGS = --32

# Linker flags
LDFLAGS = -T boot/linker.ld -m elf_i386

# Directories
SRC = src
BOOT = boot

# Output
KERNEL_BIN = yrul.bin

# Default target
all: $(KERNEL_BIN)

# Build kernel binary
$(KERNEL_BIN): $(BOOT)/boot.o $(SRC)/kernel.o $(SRC)/interrupts.o \
               $(SRC)/pic_acknowledge.o $(SRC)/io.o $(SRC)/keyboard.o \
               $(SRC)/keyboard_interrupt.o
	$(LD) $(LDFLAGS) -o $@ $^

# Bootloader
$(BOOT)/boot.o: $(BOOT)/boot.s
	$(AS) $(ASFLAGS) -o $@ $<

# Assembly files
$(SRC)/%.o: $(SRC)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

# C source files
$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean build files
clean:
	rm -f $(SRC)/*.o $(BOOT)/*.o $(KERNEL_BIN)

# Run targets
run: all
	@echo "=== Starting Yrul OS ==="
	@echo "System features:"
	@echo "- Keyboard input on line 22"
	@echo "- Command processing"
	@echo "- Automatic interrupt/polling fallback"
	@echo "- Press Ctrl+C to exit"
	@echo "========================"
	qemu-system-i386 -kernel $(KERNEL_BIN) -m 32M -no-reboot -no-shutdown

run-debug: all
	@echo "=== Debug Mode ==="
	@echo "Monitor: Ctrl+Alt+2"
	@echo "Display: Ctrl+Alt+1"
	@echo "=================="
	qemu-system-i386 -kernel $(KERNEL_BIN) -m 32M -no-reboot -no-shutdown -monitor stdio

# ISO creation (for CD/DVD boot)
iso: all
	mkdir -p iso/boot/grub
	cp $(KERNEL_BIN) iso/boot/
	cp boot/grub.cfg iso/boot/grub/
	grub-mkrescue -o yrul.iso iso
	rm -rf iso

# Install to USB (requires sudo and USB device path)
install-usb: all
	@echo "WARNING: This will overwrite the target device!"
	@echo "Usage: make install-usb USB_DEVICE=/dev/sdX"
	@if [ -z "$(USB_DEVICE)" ]; then \
		echo "Error: USB_DEVICE not specified"; \
		exit 1; \
	fi
	sudo dd if=$(KERNEL_BIN) of=$(USB_DEVICE) bs=512

# Help target
help:
	@echo "Yrul OS Build System"
	@echo "===================="
	@echo "Targets:"
	@echo "  all        - Build kernel binary"
	@echo "  clean      - Clean build files"
	@echo "  run        - Run in QEMU"
	@echo "  run-debug  - Run with QEMU monitor"
	@echo "  iso        - Create bootable ISO"
	@echo "  install-usb- Install to USB (requires USB_DEVICE=/dev/sdX)"
	@echo "  help       - Show this help"

.PHONY: all clean run run-debug iso install-usb help
