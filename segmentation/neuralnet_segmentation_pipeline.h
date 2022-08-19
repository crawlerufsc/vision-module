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
#include "../model/vision_formats.h"

template <typename T>
class NeuralNetSegmentationPipeline : public ProcessPipeline<T>
{
private:
    SourceCamera *input;
    segNet *net;
    OccupancyGrid<T> *ocgrid;
    ProcHandler *procHandler;
    Logger *logger;

    T *imgOverlay = NULL;
    T *imgMask = NULL;

    int2 inputSize;
    int2 overlaySize;
    int2 maskSize;

    std::string ignoreClass;
    segNet::FilterMode filterMode;

    bool processSegmentation(T *frame);

protected:
    bool allocBuffers(int width, int height, uint32_t flags);
    bool initialize() override;
    T *captureNextFrame() override;
    void transmitOriginal(T *) override;
    void process(T *);
    void onTerminate() override;    

public:
    NeuralNetSegmentationPipeline(SourceCamera *input, segNet *net, OccupancyGrid<T> *ocgrid, ProcHandler *procHandler, Logger *logger);
};

#endif