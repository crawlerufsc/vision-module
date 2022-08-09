# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# declare my-recognition project
project(utils-test)
set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

link_directories(/usr/lib/aarch64-linux-gnu/tegra)

SET(TARGET_H
    ../../../utils/dropping_queue.h
)


add_executable(
    queue-tst
    dropping_queue_test.cpp 
)

target_link_libraries (queue-tst ${CMAKE_THREAD_LIBS_INIT})