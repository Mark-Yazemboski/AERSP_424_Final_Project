#!/bin/bash

# Define build directory
BUILD_DIR="build"

# Remove existing build directory
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning up existing build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
echo "Creating build directory..."
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure the project
echo "Running CMake configuration..."
cmake .. -G "Visual Studio 17 2022"

# Build the project
echo "Building the project..."
cmake --build .

# Notify completion
echo "Build completed!"