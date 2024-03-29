#ifndef __SOURCE_IMAGE_DATASET_H
#define __SOURCE_IMAGE_DATASET_H

#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
// #include <opencv2/core.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/highgui.hpp>
#include <jetson-utils/loadImage.h>
#include <chrono>
#include <thread>

#include "source_camera.h"
#include "../model/vision_formats.h"

class SourceImageDatasetImpl : public SourceCamera
{
private:
    std::vector<std::string> *input;
    uint32_t width;
    uint32_t height;
    uint32_t frameRate;
    uint32_t repeat_for;
    uint32_t repeat_for_pos;
    int pos;
    SourceImageFormat *cudaBuffer;

private:
    void delayFrameRate() {
        if (frameRate <= 0) return;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000/frameRate));
    }

public:
    SourceImageDatasetImpl(uint32_t width, uint32_t height, uint32_t frameRate)
    {
        input = new std::vector<std::string>();
        this->width = width;
        this->height = height;
        this->pos = 0;
        this->repeat_for = 1;
        this->repeat_for_pos = 0;
        this->frameRate = frameRate;
        cudaMalloc((void **)&cudaBuffer, sizeof(SourceImageFormat) * width * height);
    }

    ~SourceImageDatasetImpl()
    {
        delete input;
        cudaFree(cudaBuffer);
    }

    SourceImageDatasetImpl *AddSource(std::string path)
    {
        input->push_back(path);
        return this;
    }

    virtual uint32_t GetWidth() override
    {
        return width;
    }

    virtual uint32_t GetHeight() override
    {
        return height;
    }

    //TODO: colocar como método base
    virtual uint32_t GetFrameRate() 
    {
        return frameRate;
    }

    virtual SourceImageDatasetImpl * RepeatFrame(uint32_t times)
    {
        this->repeat_for = times;
        return this;
    }


    virtual void *Capture(uint64_t timeout = UINT64_MAX) override
    {
        if (pos >= input->size())
        {
            pos = 0;
        }

        std::string filename = input->at(pos);
        SourceImageFormat *imgPtr = NULL;
        int w = width;
        int h = height;
        if (!loadImage(filename.c_str(), &imgPtr, &w, &h))
        {
            printf("failed to load image '%s'\n", filename.c_str());
            return 0;
        }
 
        this->repeat_for_pos++;

        if (this->repeat_for_pos >= this->repeat_for) {
            this->repeat_for_pos = 0;
            this->pos++;
        }

        // return (void *)cudaBuffer;
        delayFrameRate();
        return (void *)imgPtr;
    }

    virtual float *CaptureRGBA(uint64_t timeout = 100000)
    {
        return (float *)Capture(timeout);
    }

    virtual bool IsStreaming() override
    {
        return true;
    }

    virtual void Close() override
    {
    }

    void initWithOptions(videoOptions &options) override
    {
    }
};

#endif