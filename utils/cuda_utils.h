#ifndef _CUDA_UTILS_H
#define _CUDA_UTILS_H

#include <jetson-utils/cudaMappedMemory.h>

template <typename T>
class CudaUtils
{
public:
    static T *allocCUDABuffer(int width, int height)
    {
        int2 size = make_int2(width, height);
        T *ptr = nullptr;
        if (!cudaAllocMapped(&ptr, size))
        {
            LogError("segnet:  failed to allocate CUDA memory (%ux%u)\n", width, height);
        }
        return ptr;
    }
};


#endif