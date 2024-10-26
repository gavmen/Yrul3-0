# Yrul Operating System

Yrul is a simple operating system built from scratch as a learning project. The goal of Yrul is to understand the inner workings of low-level programming, kernel development, and systems programming by developing a basic operating system from the ground up.

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Getting Started](#getting-started)
<!-- 4. [Building Yrul](#building-yrul)
5. [Running Yrul](#running-yrul)
6. [Project Structure](#project-structure)
7. [Next Steps](#next-steps)
8. [License](#license) -->

## Introduction
Yrul is designed to be a minimalistic operating system, gradually built up from a simple "Hello World" kernel to a more complex system with basic memory management, multitasking, and hardware interaction. It's a perfect starting point for anyone looking to dive into operating system development and gain hands-on experience with low-level programming.

## Features
- Basic bootloader using GRUB
- Simple "Hello, Yrul!" kernel written in C and assembly
- Multiboot-compliant kernel entry point
- Cross-compilation setup for building the OS

## Getting Started

### Prerequisites
To build and run Yrul, you'll need:
- A Unix-based system (Linux or macOS)
- Cross-compiler toolchain (`i386-elf-gcc`, `i386-elf-ld`)
- Development tools: `make`, `gcc`, `bison`, `flex`, etc.
- QEMU (for emulating the OS)

### Setting Up the Cross-Compiler
Follow these steps to set up the cross-compiler:

1. **Install Dependencies** (Linux):
   ```bash
   sudo apt update
   sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
   ```

Download and Build binutils and gcc: Follow the instructions in this guide to set up i386-elf-gcc.

Add the cross-compiler to your PATH:
   ```bash
export PATH="$HOME/cross-compiler/bin:$PATH"
   ```