#include <memory>
#include <string>
#include <iostream>

#include <stdlib.h>
#include <string.h>

#include <jetson-utils/videoSource.h>
#include <jetson-utils/videoOutput.h>
#include <../../acquisition/source_camera.h>
#include <../../acquisition/source_video_dataset.h>
#include <../../log/logger.h>
#include <../../communication/stream_server.h>

#define SourceImageFormat uchar3

extern Logger *NewDebugLoggerInstance();

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "use %s <file> <stream-listerner IP> <stream-listener-port>\n\nex: %s 10.0.0.150 1234\n\n", argv[0], argv[0]);
        exit(1);
    }

    Logger *logger = NewDebugLoggerInstance();
    StreamServer *output = new StreamServer("Video", 20001, logger);
    SourceCamera *input = new SourceVideoDatasetImpl(argv[1],1920,1080);

    fprintf(stdout, "NOW run the client as:\n\n");
    fprintf(stdout, "gst-launch-1.0 -v udpsrc port=%s  caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! autovideosink", argv[3]);
    fputs("\n\nthen press ENTER\n\n",stdout);
    std::cin.get();


    output->Start();
    output->CreateOutputStream(argv[2], atoi(argv[3]));
   
    int w = input->GetWidth();
    int h = input->GetHeight();
    bool loop_run = true;
    while (loop_run)
    {
        SourceImageFormat *frame = (SourceImageFormat *)input->Capture(10000);
        if (frame == NULL)
        {
            if (!input->IsStreaming())
                loop_run = false;
        }
        else
        {
            output->NewFrame(frame, w, h);
        }
    }

    output->Wait();
    // delete source;
    // output->Stop();
    // delete output;
    // delete logger;
}