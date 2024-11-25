cmake_minimum_required(VERSION 3.10)

# Set your project name and version
project(AERSP_424_FINAL_PROJECT VERSION 1.0)

# Add the path to the SFML CMake module
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/third_party/SFML-2.4.0/cmake/Modules" ${CMAKE_MODULE_PATH})

# Set the SFML directory explicitly (if needed)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/third_party/SFML-2.4.0/cmake/Modules")

# Find the SFML package and its components (graphics, window, system)
find_package(SFML 2.4 REQUIRED graphics window system)

# Include the SFML header files
include_directories("${CMAKE_SOURCE_DIR}/third_party/SFML-2.4.0/include/SFML")

# Add the SFML library path for Windows
# Make sure the path to your SFML libraries (.lib or .dll) is correct
link_directories("${CMAKE_SOURCE_DIR}/third_party/SFML-2.4.0/lib")

# Add your executable (e.g., main.cpp)
add_executable(MyExecutable main.cpp)

# Link the SFML libraries to your executable
target_link_libraries(MyExecutable sfml-graphics sfml-window sfml-system)

# Optionally, set the output directory for your executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the directory containing SFML DLLs to the runtime path
# This is helpful if you don't want to manually copy the DLLs to the executable folder
set(SFML_DLL_DIR "${CMAKE_SOURCE_DIR}/third_party/SFML-2.4.0/bin")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# You can also add the SFML DLL path to your PATH environment variable for the runtime
# (This step is optional and only needed if you want to add it to the PATH)
# set(ENV{PATH} "$ENV{PATH}:${SFML_DLL_DIR}")

# To copy the DLLs into the build folder, uncomment the following
# configure_file("${SFML_DLL_DIR}/sfml-graphics.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sfml-graphics.dll" COPYONLY)
# configure_file("${SFML_DLL_DIR}/sfml-window.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sfml-window.dll" COPYONLY)
# configure_file("${SFML_DLL_DIR}/sfml-system.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sfml-system.dll" COPYONLY)
