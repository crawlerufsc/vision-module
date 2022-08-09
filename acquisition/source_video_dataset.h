#ifndef __SOURCE_VIDEO_DATASET_H
#define __SOURCE_VIDEO_DATASET_H

#include <string>
#include <iostream>
#include <string.h>
#include <jetson-utils/videoSource.h>
#include <jetson-utils/gstCamera.h>
#include <sstream>
#include "source_camera.h"
#include "../utils/file_utils.h"

class SourceVideoDatasetImpl : public SourceCamera
{
private:
    videoSource *input;

    void buildInput(std::string path, uint32_t width, uint32_t height)
    {
        if (!FileUtils::fileExists(path)) {
            LogError("unable to find %s\n", path.c_str());
            exit(1);
        }
        videoOptions options;
        options.ioType = videoOptions::INPUT;
        options.zeroCopy = true;
        std::ostringstream device;
        device << "file://" << path;
        options.resource = device.str().c_str();
        options.codec = videoOptions::CODEC_MPEG4;

        if (width > 0)
            options.width = width;

        if (height > 0)
            options.height = height;

        input = videoSource::Create(options);
    }

public:
    SourceVideoDatasetImpl(std::string path)
    {
        buildInput(path, -1, -1);
    }

    SourceVideoDatasetImpl(std::string path, uint32_t width, uint32_t height)
    {
        buildInput(path, width, height);
    }

    ~SourceVideoDatasetImpl()
    {
        Close();
        delete input;
    }

    void initWithOptions(videoOptions &options) override
    {

        if (input != nullptr)
        {
            input->Close();
            delete input;
        }

        input = videoSource::Create(options);
    }

    uint32_t GetWidth() override
    {
        return input->GetWidth();
    }

    uint32_t GetHeight() override
    {
        return input->GetHeight();
    }

    void *Capture(uint64_t timeout = UINT64_MAX) override
    {
        uchar3 *imgptr = NULL;

        if (!input->Capture(&imgptr, timeout))
        {
            // check for EOS
            if (!input->IsStreaming())
            {
                LogError("not streaming\n");
                return NULL;
            }

            LogError("failed to capture video frame\n");
            return NULL;
        }

        return (void *)imgptr;
    }

    float *CaptureRGBA(uint64_t timeout = UINT64_MAX) override
    {
        float4 *imgptr = NULL;

        if (!input->Capture(&imgptr, timeout))
        {
            // check for EOS
            if (!input->IsStreaming())
            {
                LogError("not streaming\n");
                return NULL;
            }

            LogError("failed to capture video frame\n");
            return NULL;
        }

        return (float *)imgptr;
    }

    void Close() override
    {
        input->Close();
    }

    bool IsStreaming() override
    {
        return input->IsStreaming();
    }
};

#endif