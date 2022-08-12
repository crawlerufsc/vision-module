# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# declare my-recognition project
project(tst-og)

# import jetson-inference and jetson-utils packages.
# note that if you didn't do "sudo make install"
# while building jetson-inference, this will error.
find_package(OpenCV REQUIRED)

# CUDA is required
find_package(CUDA)

message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

set(CMAKE_CXX_STANDARD 14)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

link_directories(/usr/lib/aarch64-linux-gnu/tegra)

SET(TARGET_H
    ../../model/vision_formats.h
    ../../utils/image_utils.h
    ../../occupancy_grid/occupancy_grid.h
    ../../occupancy_grid/occupancy_grid_impl.h
)

add_executable(
    tst-og
    tst-og.cpp
)

# link my-recognition to jetson-inference library
target_link_libraries(tst-og ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})