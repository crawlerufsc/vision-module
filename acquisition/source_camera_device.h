#ifndef __SOURCE_CAMERA_DEVICE_H
#define __SOURCE_CAMERA_DEVICE_H

#include <string>
#include <iostream>
#include <string.h>
#include <jetson-utils/videoSource.h>

#include "source_camera.h"

class SourceCameraBuilder
{
private:
    videoOptions *options;
    SourceCamera *source;

public:
    SourceCameraBuilder(SourceCamera *source)
    {
        options = new videoOptions();
        options->ioType = videoOptions::INPUT;
        options->zeroCopy = true;
        this->source = source;
    }

    SourceCameraBuilder *device(std::string device)
    {
        options->resource = device.c_str();
        return this;
    }

    SourceCameraBuilder *withSize(int width, int height)
    {
        options->width = width;
        options->height = height;
        return this;
    }
    SourceCameraBuilder *codec(videoOptions::Codec codec)
    {
        options->codec = codec;
        return this;
    }
    SourceCameraBuilder *setFps(float fps)
    {
        options->frameRate = fps;
        return this;
    }
    SourceCameraBuilder *setBitrate(uint32_t bitrate)
    {
        options->bitRate = bitrate;
        return this;
    }
    SourceCameraBuilder *gpuOnly()
    {
        options->zeroCopy = false;
        return this;
    }

    SourceCamera *build()
    {
        this->source->initWithOptions(*options);
        return this->source;
    }
};

class SourceCameraDeviceImpl : public SourceCamera
{
    videoSource *input;
    bool available;

public:
    SourceCameraDeviceImpl()
    {
        available = false;
    }

    void initWithOptions(videoOptions &options) override
    {
        input = videoSource::Create(options);

        available = true;

        if (!input || !input->Open())
        {
            LogError("failed to open USB camera");
            available = false;
        }
    }

    bool IsAvailable() override
    {
        return available;
    }

    ~SourceCameraDeviceImpl()
    {
        Close();
        delete input;
    }

    uint32_t GetWidth() override
    {
        if (!available)
            LogError("camera not available\n");
        return input->GetWidth();
    }

    uint32_t GetHeight() override
    {
        if (!available)
            LogError("camera not available\n");
        return input->GetHeight();
    }

    void *Capture(uint64_t timeout = UINT64_MAX) override
    {
        if (!available)
            LogError("camera not available\n");

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
        if (!available)
            LogError("camera not available\n");

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
        if (!available)
            LogError("camera not available\n");

        input->Close();
    }

    bool IsStreaming() override
    {
        if (!available)
            LogError("camera not available\n");

        return input->IsStreaming();
    }

    static SourceCameraBuilder *begin()
    {
        SourceCamera *source = new SourceCameraDeviceImpl();
        return new SourceCameraBuilder(source);
    }
};

#endif