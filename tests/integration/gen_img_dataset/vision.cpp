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
#include <fstream>

#include <opencv2/opencv.hpp>
#include <jetson-utils/videoSource.h>
#include <jetson-utils/videoOutput.h>
#include <jetson-utils/cudaOverlay.h>
#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-utils/imageWriter.h>
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
#include "../../../utils/image_utils.h"

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

class ImageDatasetGeneration : public ProcHandler
{
private:
    int num_frame_skip;
    int num_images;
    int img_count = 0;
    int frame_count;
    bool should_save_segmentation;

    std::string genSegmentedFileName(int i)
    {
        std::stringstream ss;
        ss << "seg_" << i << ".jpg";
        return ss.str();
    }
    std::string genOriginalFileName(int i)
    {
        std::stringstream ss;
        ss << "orig_" << i << ".jpg";
        return ss.str();
    }
    void writeFile(std::string file, uchar3 *frame, uint32_t width, uint32_t height)
    {
        std::ofstream fout;
        fout.open(file, ios::binary | ios::out);
        for (int i = 0; i < width * height; i++)
        {
            fout << frame[i].x;
            fout << frame[i].y;
            fout << frame[i].z;
        }
        fout.close();
    }
    void writeFile2(std::string file, uchar3 *frame, uint32_t width, uint32_t height)
    {
        videoOptions options;
        imageWriter *writer = imageWriter::Create(file.c_str(), options);
        writer->Render(frame, width, height, imageFormat::IMAGE_RGB8);
        delete writer;
    }

public:
    ImageDatasetGeneration(int num_frame_skip, int num_images)
    {
        this->num_frame_skip = num_frame_skip;
        this->num_images = num_images;
        this->frame_count = num_frame_skip;
        this->should_save_segmentation = false;
    }
    void FrameSkipCaptureError() override {}
    void FrameSkipMemoryFault() override{};
    void FrameSkipNetError() override {}
    void FrameSkipSegmentationOverlayError() override {}
    void FrameSkipSegmentationMaskError() override {}
    void FrameMask(uchar3 *result_value, uint32_t width, uint32_t height) override {}
    void FrameOccupancyGrid(uchar3 *frame, uint32_t width, uint32_t height) override
    {
    }
    void FrameCaptured(SourceImageFormat *frame, uint32_t width, uint32_t height) override
    {
        if (frame != NULL && frame_count >= num_frame_skip)
        {
            frame_count = 0;
            writeFile2(genOriginalFileName(img_count), frame, width, height);
            img_count++;
            num_images--;
            should_save_segmentation = true;
        }

        frame_count++;
    }
    void FrameSegmentation(SourceImageFormat *frame, uint32_t width, uint32_t height) override
    {
        if (frame != NULL && should_save_segmentation)
        {
            writeFile2(genSegmentedFileName(img_count - 1), frame, width, height);
        }

        if (img_count > num_images)
        {
            visionProc->terminate();
        }
    }
};

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "use %s <file> <# frame to skip> <# images>", argv[0]);
        exit(1);
    }

    int num_frame_skip = atoi(argv[2]);
    int num_images = atoi(argv[3]);
    int img_count = 0;

    SourceCamera *camera = new SourceVideoDatasetImpl(argv[1], 800, 600);

    OccupancyGrid<SourceImageFormat> *computeOG = new OccupancyGridImpl<SourceImageFormat>();

    ProcHandler *procHandler = new ImageDatasetGeneration(num_frame_skip, num_images);

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
