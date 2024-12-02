#Sets the minimum cmake version (Chat gpt set it at pie because its probably hungry)
cmake_minimum_required(VERSION 3.14)

#Sets the name of the project
project(AERSP424_Final_Project)

#Sets the compiler version. I think I can go higher, but if I have learned something from doing this project
#Its dont touch something thats working :D
set(CMAKE_CXX_STANDARD 17)

#Includes directories for the header files, and pictures, and the graphics library
include_directories(${CMAKE_SOURCE_DIR}/Libraries/include)
include_directories(${CMAKE_SOURCE_DIR}/PICS)
include_directories(${CMAKE_SOURCE_DIR}/Header_Files)

#Links the directory of the graphics library to the project
link_directories(${CMAKE_SOURCE_DIR}/Libraries/lib)

#Sets the variable SOURSES to the sourse files, which for this project is just play.cpp
set(SOURCES
    "Source_Files/Play.cpp"
)

#Adds stb_image.cpp for stb_image functionality
set(STB_IMAGE_SOURCE ${CMAKE_SOURCE_DIR}/Libraries/include/stb/stb_image.h)

#Makes an exicutable called Play.exe, and links the SOURCES, and stb stuff
add_executable(Play ${SOURCES} ${STB_IMAGE_SOURCE} "Source_Files/stb.cpp")

#Copies the PICS folder to the build directory
add_custom_command(
    TARGET Play POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/PICS $<TARGET_FILE_DIR:Play>/PICS
)

#Links OpenGL and GLFW libraries
find_package(OpenGL REQUIRED)
target_link_libraries(Play OpenGL::GL glfw3.lib)
