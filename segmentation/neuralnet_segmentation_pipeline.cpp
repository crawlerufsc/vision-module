#include "neuralnet_segmentation_pipeline.h"
#include "../control/process_handler.h"
#include "../utils/cuda_utils.h"

NeuralNetSegmentationPipeline::NeuralNetSegmentationPipeline(SourceCamera *input, segNet *net, OccupancyGrid *ocgrid, ProcHandler *procHandler, Logger *logger) : ProcessPipeline()
{
    this->input = input;
    this->net = net;
    this->ocgrid = ocgrid;
    this->procHandler = procHandler;
    this->logger = logger;

    this->ignoreClass = "void";
    this->filterMode = segNet::FilterModeFromStr("linear");
}

SourceImageFormat *NeuralNetSegmentationPipeline::captureNextFrame()
{
    SourceImageFormat *frame = (SourceImageFormat *)input->Capture(10000);
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

void NeuralNetSegmentationPipeline::transmitOriginal(SourceImageFormat *frame)
{
    procHandler->FrameCaptured(frame, input->GetWidth(), input->GetHeight());
}

bool NeuralNetSegmentationPipeline::processSegmentation(SourceImageFormat *frame)
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

void NeuralNetSegmentationPipeline::process(SourceImageFormat *frame)
{
    if (!processSegmentation(frame))
        return;

    logger->info("frame processed");
    procHandler->FrameSegmentation(imgOverlay, overlaySize.x, overlaySize.y);
    procHandler->FrameMask(imgMask, maskSize.x, maskSize.y);

    char *occupancyGrid = ocgrid->ComputeOcuppancyGrid(imgMask, ocgrid->GetWidth(), ocgrid->GetHeight());
    
    for (int i = 0; i < ocgrid->GetWidth() * ocgrid->GetHeight(); i++)
    {
        imgOG[i] = make_uchar3(occupancyGrid[i], 0, 0);
    }

    logger->info("OG computed");
    procHandler->FrameOccupancyGrid(imgOG, occupancyGridSize.x, occupancyGridSize.y);
}

bool NeuralNetSegmentationPipeline::initialize()
{
    CUDA_FREE_HOST(imgOverlay);
    CUDA_FREE_HOST(imgOG);

    int width = input->GetWidth();
    int height = input->GetHeight();

    inputSize = make_int2(width, height);
    overlaySize = make_int2(width, height);
    maskSize = make_int2(width, height);
    occupancyGridSize = make_int2(ocgrid->GetWidth(), ocgrid->GetHeight());

    imgOverlay = CudaUtils::allocCUDABuffer(overlaySize.x, overlaySize.y);
    imgOG = CudaUtils::allocCUDABuffer(occupancyGridSize.x, occupancyGridSize.y);
    imgMask = CudaUtils::allocCUDABuffer(maskSize.x, maskSize.y);

    if (imgOverlay == nullptr)
    {
        LogError("failed to allocate CUDA memory for overlay image (%ux%u)\n", overlaySize.x, overlaySize.y);
        return false;
    }
    if (imgOG == nullptr)
    {
        LogError("failed to allocate CUDA memory for occupancy grid (%ux%u)\n", occupancyGridSize.x, occupancyGridSize.y);
        return false;
    }
    if (imgMask == nullptr)
    {
        LogError("failed to allocate CUDA memory for image mask (%ux%u)\n", maskSize.x, maskSize.y);
        return false;
    }
    logger->info("buffers allocated - processing for width x height: %ux%u, occupancy grid %ux%u, mask %ux%u ",
                 input->GetWidth(), input->GetHeight(), occupancyGridSize.x, occupancyGridSize.y, maskSize.x, maskSize.y);

    return true;
}

void NeuralNetSegmentationPipeline::onTerminate()
{
    logger->info("process termination requested");
}