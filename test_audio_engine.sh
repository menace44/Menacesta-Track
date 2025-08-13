#!/bin/bash

# Audio Engine Test Script
# This script builds and runs the AudioEngine tests

echo "=== Audio Engine Test Suite ==="

# Create build directory
mkdir -p build-test
cd build-test

# Configure with CMake
echo "Configuring build..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build the project
echo "Building project..."
cmake --build . --config Debug

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    
    # Run the main application to test basic functionality
    echo "Running basic functionality test..."
    ./AuroraDAW &
    APP_PID=$!
    
    # Wait a moment for initialization
    sleep 2
    
    # Kill the app (we just wanted to see if it starts)
    kill $APP_PID
    
    echo "Basic test completed!"
else
    echo "Build failed!"
    exit 1
fi

cd ..
echo "=== Test Suite Complete ==="
