#ifndef _CUDA_UTILS_H
#define _CUDA_UTILS_H

#include "../model/vision_formats.h"
#include <jetson-utils/cudaMappedMemory.h>

class CudaUtils
{
public:
    static SourceImageFormat *allocCUDABuffer(int width, int height)
    {
        int2 size = make_int2(width, height);
        SourceImageFormat *ptr = nullptr;
        if (!cudaAllocMapped(&ptr, size))
        {
            LogError("segnet:  failed to allocate CUDA memory (%ux%u)\n", width, height);
        }
        return ptr;
    }
};


#endif