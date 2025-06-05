#!/bin/bash

echo "Testing Shadow Segmentation project structure..."

# Check if all required files exist
files_to_check=(
    "CMakeLists.txt"
    "src/main.cpp"
    "src/mainwindow.cpp"
    "src/shadowledentifier.cpp"
    "include/mainwindow.h"
    "include/shadowledentifier.h"
    "build.sh"
    "build.bat"
    "README.md"
)

missing_files=0

for file in "${files_to_check[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file exists"
    else
        echo "✗ $file is missing"
        missing_files=$((missing_files + 1))
    fi
done

if [ $missing_files -eq 0 ]; then
    echo "All required files are present!"
    echo "You can now build the project using ./build.sh"
else
    echo "Missing $missing_files files. Please ensure all files are created."
fi
