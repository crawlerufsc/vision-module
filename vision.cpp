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
#include "occupancy_grid/occupancy_grid_impl.h"
#include "control/process_handler.h"
#include "log/logger.h"
#include "segmentation/neuralnet_segmentation_pipeline.h"

using namespace std;
using namespace cv;
using namespace chrono;

#define DEBUG 1
#define MqttHost "10.0.0.4"
#define MqttPort 1883

extern ProcHandler *NewProcHandlerImplInstance(Logger *logger);
ProcessPipeline<SourceImageFormat> *visionProc;

#ifdef DEBUG
extern Logger *NewPubSubLoggerInstance(const char *hostIp, int port);
Logger *logger = NewPubSubLoggerInstance(MqttHost, MqttPort);
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
    SourceCamera *camera = nullptr;

    logger->publishModuleStatus(false);
    logger->publishCameraStatus(false);

    while (camera == nullptr)
    {
        camera = SourceCameraUSBImpl::begin()
                     ->device("/dev/video0")
                     ->withSize(640, 480)
                     ->build();

        if (!camera->IsAvailable())
        {
            logger->error("The camera device is not properly connected");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            delete camera;
            camera = nullptr;
        }
    }

    logger->info("camera initialized");
    logger->publishCameraStatus(true);

    OccupancyGrid<SourceImageFormat> *computeOG = new OccupancyGridImpl<SourceImageFormat>();

    ProcHandler *procHandler = NewProcHandlerImplInstance(logger);

    // segNet *net = segNet::Create(nullptr,
    //                              "net/rtkmodel_test.onnx",
    //                              "net/classes.txt",
    //                              "net/colors.txt",
    //                              "data",
    //                              "score_fr_21_classes",
    //                              10,
    //                              precisionType::TYPE_FP16,
    //                              deviceType::DEVICE_GPU,
    //                              true);

    logger->info("initializing segmentation neural network");

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


    logger->info("serving vision");
    logger->publishModuleStatus(true);

    visionProc->run();
    return 0;
}
