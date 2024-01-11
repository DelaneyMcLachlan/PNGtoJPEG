cmake_minimum_required(VERSION 3.10)
project(PNGtoJPEG)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the libpng package
find_package(PNG REQUIRED)

# Attempt to find libjpeg
find_package(JPEG)
if(NOT JPEG_FOUND)
    # If not found, use find_library
    find_library(JPEG_LIBRARY NAMES jpeg)
endif()

# Add your source files including new class implementations
add_executable(PNGtoJPEG main.cpp PNGReader.cpp JPEGWriter.cpp ColourConverter.cpp ImageUtility.cpp)

# Link against libpng and libjpeg
target_link_libraries(PNGtoJPEG PNG::PNG ${JPEG_LIBRARY})

