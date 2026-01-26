# MOS6502 Emulator Build System

# Compiler and flags
CC := "clang"
CFLAGS := "-Wall -Wextra -Werror -std=c11 -Iinclude -Iplatforms"
DEBUG_FLAGS := "-g -O0 -DDEBUG"
RELEASE_FLAGS := "-O3"

# Directories
SRC_DIR := "src"
PLATFORM_DIR := "platforms"
BUILD_DIR := "build"
BIN_DIR := "bin"

# Source files
SOURCES := SRC_DIR + "/main.c " + SRC_DIR + "/mos6502.c " + SRC_DIR + "/bus.c " + SRC_DIR + "/instructions.c " + PLATFORM_DIR + "/platform_basic.c"

# Output binary
TARGET := BIN_DIR + "/mos6502"

# Default recipe - shows available commands
default:
    @just --list

# Build release version
build: clean
    @echo "Building release version..."
    @mkdir -p {{BIN_DIR}}
    {{CC}} {{CFLAGS}} {{RELEASE_FLAGS}} {{SOURCES}} -o {{TARGET}}
    @echo "✓ Build complete: {{TARGET}}"

# Build debug version
debug: clean
    @echo "Building debug version..."
    @mkdir -p {{BIN_DIR}}
    {{CC}} {{CFLAGS}} {{DEBUG_FLAGS}} {{SOURCES}} -o {{TARGET}}
    @echo "✓ Debug build complete: {{TARGET}}"

# Run the emulator (builds if necessary)
run: build
    @echo "Running emulator..."
    @./{{TARGET}}

# Run with debug build
run-debug: debug
    @echo "Running debug build..."
    @./{{TARGET}}

# Clean build artifacts
clean:
    @echo "Cleaning build artifacts..."
    @rm -rf {{BIN_DIR}} {{BUILD_DIR}} *.dSYM
    @echo "✓ Clean complete"

# Format code (requires clang-format)
format:
    @echo "Formatting code..."
    @find src include platforms -name "*.c" -o -name "*.h" | xargs clang-format -i
    @echo "✓ Code formatted"

# Run with debugger (lldb on macOS)
gdb: debug
    lldb {{TARGET}}

# Check for memory leaks with valgrind (if installed)
valgrind: debug
    valgrind --leak-check=full --show-leak-kinds=all ./{{TARGET}}

# Display compiler and project info
info:
    @echo "Compiler: {{CC}}"
    @echo "CFLAGS: {{CFLAGS}}"
    @echo "Sources: {{SOURCES}}"
    @echo "Target: {{TARGET}}"
