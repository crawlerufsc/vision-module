#include "neuralnet_segmentation_pipeline.h"
#include "../control/process_handler.h"
#include "../utils/cuda_utils.h"

template class NeuralNetSegmentationPipeline<SourceImageFormat>;

template <typename T>
NeuralNetSegmentationPipeline<T>::NeuralNetSegmentationPipeline(SourceCamera *input, segNet *net, OccupancyGrid<T> *ocgrid, ProcHandler *procHandler, Logger *logger) : ProcessPipeline<T>()
{
    this->input = input;
    this->net = net;
    this->ocgrid = ocgrid;
    this->procHandler = procHandler;
    this->logger = logger;

    this->ignoreClass = "void";
    this->filterMode = segNet::FilterModeFromStr("linear");
}

template <typename T>
T *NeuralNetSegmentationPipeline<T>::captureNextFrame()
{
    T *frame = (T *)input->Capture(10000);
    if (frame == NULL)
    {
        if (!input->IsStreaming())
        {
            terminate();
            return nullptr;
        }

        procHandler->FrameSkipCaptureError();
        logger->error("frame skipped by capture error");
        return nullptr;
    }

    return frame;
}

template <typename T>
void NeuralNetSegmentationPipeline<T>::transmitOriginal(T *frame)
{
    procHandler->FrameCaptured(frame, input->GetWidth(), input->GetHeight());
}

template <typename T>
bool NeuralNetSegmentationPipeline<T>::processSegmentation(T *frame)
{
    if (!net->Process(frame, input->GetWidth(), input->GetHeight(), ignoreClass.c_str()))
    {
        procHandler->FrameSkipNetError();
        logger->error("the neuralnet failed to process segmentation");
        return false;
    }

    logger->info("neuralnet: frame processed");

    // generate overlay
    if (!net->Overlay(imgOverlay, overlaySize.x, overlaySize.y, filterMode))
    {
        procHandler->FrameSkipSegmentationOverlayError();
        logger->error("the neuralnet failed to process segmentation overlay");
        return false;
    }

    logger->info("neuralnet: overlay");

    // generate mask
    if (!net->Mask(imgMask, maskSize.x, maskSize.y, filterMode))
    {
        procHandler->FrameSkipSegmentationMaskError();
        logger->error("the neuralnet failed to process segmentation mask");
        return false;
    }
    logger->info("neuralnet: mask");

    CUDA(cudaDeviceSynchronize());
    return true;
}

template <typename T>
void NeuralNetSegmentationPipeline<T>::process(T *frame)
{
    if (!processSegmentation(frame))
        return;

    logger->info("frame processed");
    procHandler->FrameSegmentation(imgOverlay, overlaySize.x, overlaySize.y);
    procHandler->FrameMask(imgMask, maskSize.x, maskSize.y);

    ocgrid->ComputeOcuppancyGrid(imgMask, ocgrid->GetWidth(), ocgrid->GetHeight());

    logger->info("OG computed");
    procHandler->FrameOccupancyGrid(ocgrid->GetResult(), ocgrid->GetWidth(), ocgrid->GetHeight());
}

template <typename T>
bool NeuralNetSegmentationPipeline<T>::initialize()
{
    CUDA_FREE_HOST(imgOverlay);
    CUDA_FREE_HOST(imgMask);

    int width = input->GetWidth();
    int height = input->GetHeight();

    inputSize = make_int2(width, height);
    overlaySize = make_int2(width, height);
    maskSize = make_int2(width / 2, height / 2);
   

    imgOverlay = CudaUtils<T>::allocCUDABuffer(overlaySize.x, overlaySize.y);
    imgMask = CudaUtils<T>::allocCUDABuffer(maskSize.x, maskSize.y);

    if (imgOverlay == nullptr)
    {
        LogError("failed to allocate CUDA memory for overlay image (%ux%u)\n", overlaySize.x, overlaySize.y);
        return false;
    }
    if (imgMask == nullptr)
    {
        LogError("failed to allocate CUDA memory for image mask (%ux%u)\n", maskSize.x, maskSize.y);
        return false;
    }
    logger->info("buffers allocated - processing for width x height: %ux%u, occupancy grid %ux%u, mask %ux%u ",
                 input->GetWidth(), input->GetHeight(), ocgrid->GetWidth(), ocgrid->GetHeight(), maskSize.x, maskSize.y);

    return true;
}

template <typename T>
void NeuralNetSegmentationPipeline<T>::onTerminate()
{
    logger->info("process termination requested");
}