#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>

#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>

#include <opencv2/opencv.hpp>
#include <jetson-utils/videoSource.h>
#include <jetson-utils/videoOutput.h>
#include <jetson-utils/cudaOverlay.h>
#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-inference/segNet.h>

#include "../../../model/vision_formats.h"
#include "../../../acquisition/source_image_dataset.h"
#include "../../../acquisition/source_video_dataset.h"
#include "../../../acquisition/source_camera_gst.h"
#include "../../../occupancy_grid/occupancy_grid_impl.h"
#include "../../../control/process_handler.h"
#include "../../../control/process_pipeline.h"
#include "../../../log/logger.h"
#include "../../../segmentation/neuralnet_segmentation_pipeline.h"

using namespace std;
using namespace cv;
using namespace chrono;

#define DEBUG 1

extern ProcHandler *NewProcHandlerImplInstance(Logger *logger);
ProcessPipeline<SourceImageFormat> *visionProc;


#ifdef DEBUG
extern Logger *NewDebugLoggerInstance();
Logger *logger = NewDebugLoggerInstance();
#else
Logger *logger = new Logger();
#endif

void sig_handler(int val)
{
    if (val == SIGINT)
    {
        LogVerbose("received SIGINT\n");
        visionProc->terminate();
    }
}

int main(int argc, char **argv)
{
    SourceCamera *camera = new SourceVideoDatasetImpl("/home/nvidia/20210603_162058.mp4",800,600);

    OccupancyGrid<SourceImageFormat> *computeOG = new OccupancyGridImpl<SourceImageFormat>();

    ProcHandler *procHandler = NewProcHandlerImplInstance(logger);

    segNet *net = segNet::Create(nullptr,
                                 "net/hrnet_w18.onnx",
                                 "net/classes.txt",
                                 "net/colors.txt",
                                 "input.1",
                                 "3545",                                
                                 10,
                                 precisionType::TYPE_FP16,
                                 deviceType::DEVICE_GPU,
                                 false);

    visionProc = new NeuralNetSegmentationPipeline<SourceImageFormat>(camera, net, computeOG, procHandler, logger);

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        LogError("can't catch SIGINT\n");

    visionProc->run();
    return 0;
}
