#!/bin/bash

# Aurora DAW Build Script
# Cross-platform build script for Aurora DAW

set -e

echo "🎵 Building Aurora DAW..."

# Check if JUCE directory exists
if [ ! -d "JUCE" ] || [ ! -f "JUCE/CMakeLists.txt" ]; then
    echo "❌ JUCE directory not found or incomplete!"
    echo "Please download JUCE 7.x from https://juce.com/get-juce"
    echo "and extract it to the JUCE/ directory"
    exit 1
fi

# Detect platform
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="Linux"
    GENERATOR="Unix Makefiles"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macOS"
    GENERATOR="Xcode"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
    PLATFORM="Windows"
    GENERATOR="Visual Studio 17 2022"
else
    echo "❌ Unsupported platform: $OSTYPE"
    exit 1
fi

echo "📋 Platform detected: $PLATFORM"
echo "🔧 Using generator: $GENERATOR"

# Clean previous build
if [ -d "build" ]; then
    echo "🧹 Cleaning previous build..."
    rm -rf build
fi

# Configure
echo "⚙️  Configuring CMake..."
cmake -S . -B build -G "$GENERATOR"

# Build
echo "🔨 Building..."
cmake --build build --config Release

echo "✅ Build complete!"
echo ""
echo "📁 Output location:"
if [[ "$PLATFORM" == "Windows" ]]; then
    echo "   build/Release/AuroraDAW.exe"
elif [[ "$PLATFORM" == "macOS" ]]; then
    echo "   build/Release/AuroraDAW.app"
else
    echo "   build/AuroraDAW"
fi
