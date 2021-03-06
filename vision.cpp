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

#include "model/vision_formats.h"
#include "acquisition/source_camera_gst.h"
#include "occupancy_grid/occupancy_grid.h"
#include "control/process_handler.h"
#include "log/logger.h"
#include "segmentation/neuralnet_segmentation.h"

// https://gist.github.com/jungle-cat

using namespace std;
using namespace cv;
using namespace chrono;

#define DEBUG 1

typedef uchar3 pixelType;

extern OccupancyGrid *NewOccupancyGridImplInstance();
extern ProcHandler *NewProcHandlerImplInstance(Logger *logger);
NeuralNetVision *visionProc;

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
        visionProc->Terminate();
    }
}

int main(int argc, char **argv)
{
    SourceCamera *camera = SourceCameraUSBImpl::begin()
                               ->device("/dev/video1")
                               ->withSize(640, 480)
                               ->build();

    OccupancyGrid *computeOG = NewOccupancyGridImplInstance();

    ProcHandler *procHandler = NewProcHandlerImplInstance(logger);

    segNet *net = segNet::Create(nullptr,
                                 "net/hrnet_w18.onnx",
                                 "net/classes.txt",
                                 "net/colors.txt",
                                 "data",
                                 "score_fr_21_classes",
                                 10,
                                 precisionType::TYPE_FP16,
                                 deviceType::DEVICE_GPU,
                                 true);

    visionProc = new NeuralNetVision(camera, net, computeOG, procHandler, logger);

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        LogError("can't catch SIGINT\n");

    visionProc->LoopUntilSignaled();
    return 0;
}
