# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

project(stream-test)

# import jetson-inference and jetson-utils packages.
# note that if you didn't do "sudo make install"
# while building jetson-inference, this will error.

message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED) 
find_package(PkgConfig REQUIRED)


include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)

pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)
pkg_check_modules(Mosquittopp IMPORTED_TARGET libmosquittopp REQUIRED)

link_directories(/usr/lib/aarch64-linux-gnu/tegra)

# framework lib
find_library(FRAMEWORK_LIB REQUIRED
    NAMES crawler_fw
    HINTS /usr/lib
)
include_directories(/usr/include/crawler)

add_executable(
    stream_tst
    test_open_streams.cpp
)

# link my-recognition to jetson-inference library
target_link_libraries(stream_tst
    ${FRAMEWORK_LIB}
    PkgConfig::Mosquitto
    PkgConfig::Mosquittopp
    ${CMAKE_THREAD_LIBS_INIT}
)