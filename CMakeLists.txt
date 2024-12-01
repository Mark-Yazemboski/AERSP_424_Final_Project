cmake_minimum_required(VERSION 3.14)

project(AERSP424_Final_Project)
set(CMAKE_CXX_STANDARD 17)

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR}/Libraries/include)
include_directories(${CMAKE_SOURCE_DIR}/PICS)
include_directories(${CMAKE_SOURCE_DIR}/Header_Files)

# Locate GLFW library
link_directories(${CMAKE_SOURCE_DIR}/Libraries/lib)

# Source files
set(SOURCES
    "Source_Files/Play.cpp"
)

# Add stb_image.cpp for stb_image functionality
set(STB_IMAGE_SOURCE ${CMAKE_SOURCE_DIR}/Libraries/include/stb/stb_image.h)

# Add executable
add_executable(Play ${SOURCES} ${STB_IMAGE_SOURCE} "Source_Files/stb.cpp")

# Copy the PICS folder to the build directory
add_custom_command(
    TARGET Play POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/PICS $<TARGET_FILE_DIR:Play>/PICS
)

# Link OpenGL and GLFW
find_package(OpenGL REQUIRED)
target_link_libraries(Play OpenGL::GL glfw3.lib)
