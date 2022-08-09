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

    SourceImageFormat *imgMask = NULL;      // color of each segmentation class
    SourceImageFormat *imgOverlay = NULL;   // input + alpha-blended mask
    SourceImageFormat *imgComposite = NULL; // overlay with mask next to it
    SourceImageFormat *imgOutput = NULL;    // reference to one of the above three
    SourceImageFormat *imgOG = NULL;        // overlay with mask next to it
    uint32_t visualizationFlags;

    int2 maskSize;
    int2 overlaySize;
    int2 compositeSize;
    int2 outputSize;

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
    NeuralNetSegmentationPipeline *SetVisualizationFlags(uint32_t flags);
    NeuralNetSegmentationPipeline *SetVisualizationFlags(string flags);
};

#endif