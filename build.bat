@echo off
REM Aurora DAW Build Script for Windows
REM Cross-platform build script for Aurora DAW

echo 🎵 Building Aurora DAW...

REM Check if JUCE directory exists
if not exist "JUCE\CMakeLists.txt" (
    echo ❌ JUCE directory not found or incomplete!
    echo Please download JUCE 7.x from https://juce.com/get-juce
    echo and extract it to the JUCE\ directory
    pause
    exit /b 1
)

REM Clean previous build
if exist "build" (
    echo 🧹 Cleaning previous build...
    rmdir /s /q build
)

REM Configure
echo ⚙️  Configuring CMake...
cmake -S . -B build -G "Visual Studio 17 2022"

REM Build
echo 🔨 Building...
cmake --build build --config Release

echo ✅ Build complete!
echo.
echo 📁 Output location:
echo    build\Release\AuroraDAW.exe
pause
