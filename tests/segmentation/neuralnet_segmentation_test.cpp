#include <memory>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "../../acquisition/source_image_dataset.h"
#include "../../segmentation/neuralnet_segmentation.h"
#include "../../occupancy_grid/occupancy_grid.h"
#include "../../control/process_handler.h"
#include "../../log/logger.h"

using namespace std;

class NeuralNetVisionTest : NeuralNetVision
{
public:
    NeuralNetVisionTest(SourceCamera *input, segNet *net, OccupancyGrid *ocgrid, ProcHandler *procHandler, Logger *logger) : NeuralNetVision(input, net, ocgrid, procHandler, logger)
    {
    }
    void TestLoop()
    {
        this->loop();
    }
};

class DummyOccupancyGrid : public OccupancyGrid
{
public:
    char *ComputeOcuppancyGrid(void *frame, int width, int height) override
    {
        return nullptr;
    };

    int GetWidth() override
    {
        return 0;
    }
    int GetHeight() override
    {
        return 0;
    }
};

class DummyProcHandler : public ProcHandler
{
public:
    void FrameSkipCaptureError() override
    {
    }
    void FrameSkipMemoryFault() override
    {
    }
    void FrameSkipNetError() override
    {
    }
    virtual void FrameSkipSegmentationOverlayError() override
    {
    }
    virtual void FrameSkipSegmentationMaskError() override
    {
    }
    virtual void FrameProcessResult(uchar3 *result_value, uint32_t width, uint32_t height) override
    {
    }
    virtual void FrameCaptured(SourceImageFormat *result_value, uint32_t width, uint32_t height) override
    {
    }
    virtual void FrameSegmentationSuccess(SourceImageFormat *result_value, uint32_t width, uint32_t height) override {

    }
};

extern Logger *NewDebugLoggerInstance();

int main(int argc, char **argv)
{
    SourceImageDatasetImpl *camera = new SourceImageDatasetImpl(384, 216, -1);
    camera
        ->AddSource("../../imgs/0.png")
        ->AddSource("../../imgs/1.png")
        ->AddSource("../../imgs/2.png");

    segNet *net = segNet::Create(nullptr,
                                 "net/hrnet_w18.onnx",
                                 "net/classes.txt",
                                 "net/colors.txt",
                                 "input.1",
                                 "3545",
                                 1000,
                                 precisionType::TYPE_INT8,
                                 deviceType::DEVICE_GPU,
                                 true);

    DummyOccupancyGrid *dummyog = new DummyOccupancyGrid();
    DummyProcHandler *dummyProcHandler = new DummyProcHandler();
    Logger *logger = NewDebugLoggerInstance();

    NeuralNetVisionTest *tst = new NeuralNetVisionTest(camera, net, dummyog, dummyProcHandler, logger);

    fprintf(stdout, "\nPASS\n");

    return 0;
}
