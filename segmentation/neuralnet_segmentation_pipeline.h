#ifndef _NEURALNET_SEGMENTATION_H
#define _NEURALNET_SEGMENTATION_H

#include <memory>
#include <string>
#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <mutex>

#include <jetson-utils/videoSource.h>
#include <jetson-utils/videoOutput.h>
#include <jetson-utils/cudaOverlay.h>
#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-inference/segNet.h>

#include "../acquisition/source_camera.h"
#include "../occupancy_grid/occupancy_grid.h"
#include "../control/process_handler.h"
#include "../log/logger.h"
#include "../control/process_pipeline.h"

class NeuralNetSegmentationPipeline : public ProcessPipeline
{
private:
    SourceCamera *input;
    segNet *net;
    OccupancyGrid *ocgrid;
    ProcHandler *procHandler;
    Logger *logger;

    SourceImageFormat *imgOverlay = NULL;
    SourceImageFormat *imgMask = NULL;
    SourceImageFormat *imgOG = NULL;

    int2 inputSize;
    int2 overlaySize;
    int2 maskSize;
    int2 occupancyGridSize;

    std::string ignoreClass;
    segNet::FilterMode filterMode;

    bool processSegmentation(SourceImageFormat *frame);

protected:
    bool allocBuffers(int width, int height, uint32_t flags);
    bool initialize() override;
    SourceImageFormat *captureNextFrame() override;
    void transmitOriginal(SourceImageFormat *) override;
    void process(SourceImageFormat *);
    void onTerminate() override;    

public:
    NeuralNetSegmentationPipeline(SourceCamera *input, segNet *net, OccupancyGrid *ocgrid, ProcHandler *procHandler, Logger *logger);
};

#endif