#ifndef __PROCESS_HANDLER_H
#define __PROCESS_HANDLER_H

#include "../model/vision_formats.h"

class ProcHandler
{
public:
    virtual void FrameSkipCaptureError() = 0;
    virtual void FrameSkipMemoryFault() = 0;
    virtual void FrameSkipNetError() = 0;
    virtual void FrameSkipSegmentationOverlayError() = 0;
    virtual void FrameSkipSegmentationMaskError() = 0;
    virtual void FrameMask(uchar3 *result_value, uint32_t width, uint32_t height) = 0;
    virtual void FrameOccupancyGrid(uchar3 *result_value, uint32_t width, uint32_t height) = 0;
    virtual void FrameCaptured(SourceImageFormat *result_value, uint32_t width, uint32_t height) = 0;
    virtual void FrameSegmentation(SourceImageFormat *result_value, uint32_t width, uint32_t height) = 0;
};

#endif