#!/bin/bash

# Build script for MISIS Image Processing Course

echo "Building MISIS Image Processing Course Project..."

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Building project..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo ""
    echo "Executables are available in:"
    echo "  - build/bin/ (main programs)"
    echo "  - build/bin/test/ (test programs)"
    echo ""
    echo "To run tests: cd build && ctest"
    echo ""
    echo "Example usage:"
    echo "  ./bin/lab1_raster_formats_gamma"
    echo "  ./bin/lab2_image_noise"
    echo "  ./bin/test/lab1_raster_formats_gamma_test"
else
    echo "Build failed!"
    exit 1
fi
