#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$DIR")"
TOOLCHAIN_DIR="$PROJECT_ROOT/etc/arm-gnu-toolchain"

if [ -d "$TOOLCHAIN_DIR/bin" ]; then
    echo "✅ Toolchain already exists at $TOOLCHAIN_DIR"
    exit 0
fi

echo "🔄 Downloading arm-gnu-toolchain for aarch64-none-elf..."
mkdir -p "$PROJECT_ROOT/etc"
cd "/tmp"

# Detect host architecture
ARCH=$(uname -m)
OS=$(uname -s)

if [ "$OS" = "Linux" ]; then
    if [ "$ARCH" = "x86_64" ]; then
        URL="https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-elf.tar.xz"
        FILENAME="arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-elf.tar.xz"
        DIRNAME="arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-elf"
    elif [ "$ARCH" = "aarch64" ]; then
        URL="https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-aarch64-aarch64-none-elf.tar.xz"
        FILENAME="arm-gnu-toolchain-13.3.rel1-aarch64-aarch64-none-elf.tar.xz"
        DIRNAME="arm-gnu-toolchain-13.3.rel1-aarch64-aarch64-none-elf"
    else
        echo "❌ Unsupported architecture: $ARCH"
        exit 1
    fi
else
    # Mac / OSX
    if [ "$ARCH" = "x86_64" ]; then
        URL="https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-darwin-x86_64-aarch64-none-elf.tar.xz"
        FILENAME="arm-gnu-toolchain-13.3.rel1-darwin-x86_64-aarch64-none-elf.tar.xz"
        DIRNAME="arm-gnu-toolchain-13.3.rel1-darwin-x86_64-aarch64-none-elf"
    elif [ "$ARCH" = "arm64" ]; then
        URL="https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-darwin-arm64-aarch64-none-elf.tar.xz"
        FILENAME="arm-gnu-toolchain-13.3.rel1-darwin-arm64-aarch64-none-elf.tar.xz"
        DIRNAME="arm-gnu-toolchain-13.3.rel1-darwin-arm64-aarch64-none-elf"
    else
        echo "❌ Unsupported OS/arch combo: $OS $ARCH"
        exit 1
    fi
fi

wget -q -O "$FILENAME" "$URL"
echo "📦 Extracting $FILENAME..."
tar -xf "$FILENAME"

echo "📂 Moving to $TOOLCHAIN_DIR..."
rm -rf "$TOOLCHAIN_DIR"
mv "$DIRNAME" "$TOOLCHAIN_DIR"
rm "$FILENAME"

echo "✅ Toolchain setup complete!"
