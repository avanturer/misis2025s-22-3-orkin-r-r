#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run cmake
cmake ..

# Build the project
make -j4

echo "Build completed. Run './ShadowSegmentation' to start the application."
