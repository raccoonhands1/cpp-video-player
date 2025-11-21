#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Build the project
cd build
cmake ..
cmake --build .

echo ""
echo "Build complete! Run './main --help' to see usage"
