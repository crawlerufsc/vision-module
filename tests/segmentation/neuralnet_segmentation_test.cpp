#include <memory>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "../../acquisition/source_image_dataset.h"
#include "../../segmentation/neuralnet_segmentation_pipeline.h"
#include "../../occupancy_grid/occupancy_grid.h"
#include "../../control/process_handler.h"
#include "../../log/logger.h"

using namespace std;

class NeuralNetVisionTest : public NeuralNetSegmentationPipeline<SourceImageFormat>
{
public:
    NeuralNetVisionTest(SourceCamera *input, segNet *net, OccupancyGrid<SourceImageFormat> *ocgrid, ProcHandler *procHandler, Logger *logger) : NeuralNetSegmentationPipeline<SourceImageFormat>(input, net, ocgrid, procHandler, logger)
    {
    }
    void TestLoop()
    {
        this->run();
    }
};

class DummyOccupancyGrid : public OccupancyGrid<SourceImageFormat>
{
public:
    void ComputeOcuppancyGrid(void *frame, int width, int height) override{};

    int GetWidth() override
    {
        return 0;
    }
    int GetHeight() override
    {
        return 0;
    }
    SourceImageFormat *GetResult() override
    {
        return nullptr;
    }
};

class DummyProcHandler : public ProcHandler
{
public:
    void FrameSkipCaptureError() {}
    void FrameSkipMemoryFault() {}
    void FrameSkipNetError() {}
    void FrameSkipSegmentationOverlayError() {}
    void FrameSkipSegmentationMaskError() {}
    void FrameMask(uchar3 *result_value, uint32_t width, uint32_t height) {}
    void FrameOccupancyGrid(uchar3 *result_value, uint32_t width, uint32_t height) {}
    void FrameCaptured(SourceImageFormat *result_value, uint32_t width, uint32_t height) {}
    void FrameSegmentation(SourceImageFormat *result_value, uint32_t width, uint32_t height) {}
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
                                 //  "net/hrnet_w18.onnx",
                                 // "net/classes.txt",
                                 // "net/colors.txt",
                                 //"input.1",
                                 //"3545",
                                 "net/rtkmodel_test.onnx",
                                 "net/rtkmodel_test_classes.txt",
                                 "net/rtkmodel_test_colors.txt",
                                 "input",
                                 "output",
                                 1,
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
