#ifndef _IMAGE_UTILS_H
#define _IMAGE_UTILS_H

#include <string>
#include <iostream>
#include <string.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <jetson-utils/cudaMappedMemory.h>

class ImageUtils
{
public:
    static size_t TestFileSize(std::string path)
    {
        std::ifstream file(path, std::ifstream::binary);
        file.seekg(0, file.end);
        size_t length = static_cast<size_t>(file.tellg());
        file.close();
        return length;
    }

    static void *TestReadfile(std::string filename)
    {
        std::ifstream file(filename, std::ifstream::binary);
        file.seekg(0, file.end);
        size_t length = static_cast<size_t>(file.tellg());
        file.seekg(0, file.beg);

        char *buffer = new char[length];
        file.read(buffer, length);
        file.close();
        return (void *)buffer;
    }

    static void writeFrameToPng(uchar3 * frame, int width, int height, std::string file)
    {
        // cv::Mat original_image_rgb(height, width, CV_8UC3, frame);
        // cv::Mat original_image_bgr(height, width, CV_8UC3);
        // cv::cvtColor(original_image_rgb, original_image_bgr, cv::COLOR_RGB2BGR);
        // cv::imwrite(file, original_image_bgr);

        cv::Mat original_image_rgb(height, width, CV_8UC3, frame);
        cv::imwrite(file, original_image_rgb);
    }
};

#endif