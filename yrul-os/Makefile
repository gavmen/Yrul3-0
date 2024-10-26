# Makefile for Yrul OS

TARGET = i386-elf
CC = $(TARGET)-gcc
LD = $(TARGET)-ld
AS = $(TARGET)-as
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T boot/linker.ld

# Directories
SRC = src
BOOT = boot

# Output binary
KERNEL_BIN = yrul.bin

all: $(KERNEL_BIN)

$(KERNEL_BIN): $(BOOT)/boot.o $(SRC)/kernel.o $(SRC)/interrupts.o
	$(LD) $(LDFLAGS) -o $@ $^

# Assembly file for the bootloader
$(BOOT)/boot.o: $(BOOT)/boot.s
	$(AS) -o $@ $<

# C source files
$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SRC)/*.o $(BOOT)/*.o $(KERNEL_BIN)

run: all
	qemu-system-i386 -kernel $(KERNEL_BIN)
