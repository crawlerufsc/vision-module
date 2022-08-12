#include <memory>
#include <string>
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <jetson-utils/videoSource.h>
#include <jetson-utils/videoOutput.h>
#include <../../acquisition/source_camera.h>
#include <../../acquisition/source_video_dataset.h>
#include <../../log/logger.h>
#include <../../utils/image_utils.h>

#define SourceImageFormat uchar3

extern Logger *NewDebugLoggerInstance();

std::string genFileName(int i)
{
    std::stringstream ss;
    ss << "seg_" << i << ".jpg";
    return ss.str();
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "use %s <file> <# frame to skip> <# images>", argv[0]);
        exit(1);
    }

    Logger *logger = NewDebugLoggerInstance();
    SourceCamera *input = new SourceVideoDatasetImpl(argv[1], 1920, 1080);

    int w = input->GetWidth();
    int h = input->GetHeight();
    int num_frame_skip = atoi(argv[2]);
    int num_images = atoi(argv[3]);
    bool loop_run = true;
    int img_count = 0;

    int i = num_frame_skip;
    while (loop_run)
    {
        SourceImageFormat *frame = (SourceImageFormat *)input->Capture(10000);
        if (frame != NULL && i >= num_frame_skip)
        {
            i = 0;
            ImageUtils::writeFrameToPng(frame, w, h, genFileName(img_count));
            img_count++;
            num_images--;
        }
        if (!input->IsStreaming() || num_images == 0)
            loop_run = false;
        
        i++;
    }

}