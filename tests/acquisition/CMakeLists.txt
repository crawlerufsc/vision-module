# require CMake 2.8 or greater
cmake_minimum_required(VERSION 2.8)

# declare my-recognition project
project(usb-camera-tests)

# import jetson-inference and jetson-utils packages.
# note that if you didn't do "sudo make install"
# while building jetson-inference, this will error.
find_package(jetson-utils REQUIRED)
find_package(jetson-inference REQUIRED)
find_package(OpenCV REQUIRED)

# CUDA is required
find_package(CUDA)

message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

set(CMAKE_CXX_STANDARD 14)
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(/usr/include/gstreamer-1.0 /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)
include_directories(../../aquisition)

# add directory for libnvbuf-utils to program
link_directories(/usr/lib/aarch64-linux-gnu/tegra)

# compile the my-recognition program
SET(TARGET_H
    ../../acquisition/source_camera.h
    ../../acquisition/source_camera_gst.h
    ../../acquisition/source_image_dataset.h
    ../../acquisition/source_video_dataset.h
    ../../utils/image_utils.h
)
    

cuda_add_executable(usb2file 
    test_usb_to_file.cpp
)
cuda_add_executable(usbview 
    test_usb_viewer.cpp
)
cuda_add_executable(aqtst 
    test_source_dataset.cpp
)
cuda_add_executable(usbremote 
    test_usb_remote_viewer.cpp
)
cuda_add_executable(stream_video 
    test_stream_videofile.cpp
    ../../log/logger_debug_stdout.cpp
    ../../communication/stream_server.cpp
)
cuda_add_executable(segvideo2img
    test_video2dataset_segmentation.cpp
    ../../log/logger_debug_stdout.cpp
)
# link my-recognition to jetson-inference library
target_link_libraries(usb2file jetson-inference ${OpenCV_LIBS})
target_link_libraries(usbview jetson-inference ${OpenCV_LIBS})
target_link_libraries(aqtst jetson-inference ${OpenCV_LIBS})
target_link_libraries(usbremote jetson-inference ${OpenCV_LIBS})
target_link_libraries(stream_video jetson-inference ${OpenCV_LIBS})
target_link_libraries(segvideo2img jetson-inference ${OpenCV_LIBS})


