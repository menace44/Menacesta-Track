@echo off
echo Building Menacesta Track - Standalone Version
echo =============================================

:: Create build directory
if not exist build_standalone mkdir build_standalone
cd build_standalone

:: Compile the standalone version
echo Compiling simple_mixer_standalone.cpp...
g++ -std=c++17 -o menacesta_track.exe ../src/simple_mixer_standalone.cpp

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Build successful!
echo.
echo Run the program with:
echo   .\menacesta_track.exe
echo.
pause
