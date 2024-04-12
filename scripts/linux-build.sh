#!/bin/bash

cmake_preset="clang-debug"

echo "This script is building on Linux"
echo "Building cmake preset : $cmake_preset"

# Generate project files
echo "Generating project files..."
cmake . --preset "$cmake_preset"

# Check for successful generation (optional)
if [[ $? -ne 0 ]]; then
  echo "Error: CMake project generation failed!"
  exit 1
fi

# Change directory
cd build/$cmake_preset

# Build the project
echo "Building project..."
cmake --build . -j

# Success message
echo "Project build success, use cd build/$cmake_preset"
