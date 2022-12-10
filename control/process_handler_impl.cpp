#include "process_handler.h"
#include "../communication/stream_server.h"

#include <jetson-utils/cudaMappedMemory.h>

class ProcHandlerImpl : public ProcHandler
{
    Logger *logger;
    StreamServer *originalFrameStreamServer;
    StreamServer *segmentedFrameStreamServer;
    StreamServer *maskFrameStreamServer;
    StreamServer *occupancyGridStreamServer;

private:
    void buildStreamServers(const char *pubSubHost, int pubSubPort)
    {
        originalFrameStreamServer = new StreamServer("OriginalImageStream", "/vision-module/cmd/original", logger, pubSubHost, pubSubPort);
        segmentedFrameStreamServer = new StreamServer("SegmentedFrameStream", "/vision-module/cmd/segmented", logger, pubSubHost, pubSubPort);
        maskFrameStreamServer = new StreamServer("MaskFrameStream", "/vision-module/cmd/mask", logger, pubSubHost, pubSubPort);
        occupancyGridStreamServer = new StreamServer("OGStream", "/vision-module/cmd/og", logger, pubSubHost, pubSubPort);

        // originalFrameStreamServer->Start(2000);
        // segmentedFrameStreamServer->Start(2000);
        // maskFrameStreamServer->Start(2000);
        // occupancyGridStreamServer->Start(2000);
    }
    void destroyStreamServers()
    {
        originalFrameStreamServer->Stop();
        segmentedFrameStreamServer->Stop();
        maskFrameStreamServer->Stop();
        occupancyGridStreamServer->Stop();
        delete originalFrameStreamServer;
        delete segmentedFrameStreamServer;
        delete occupancyGridStreamServer;
        delete maskFrameStreamServer;
    }

public:
    ProcHandlerImpl(Logger *logger, const char *pubSubHost, int pubSubPort)
    {
        this->logger = logger;
        buildStreamServers(pubSubHost, pubSubPort);
    }

    ~ProcHandlerImpl()
    {
        destroyStreamServers();
    }

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
    virtual void FrameMask(uchar3 *result_value, uint32_t width, uint32_t height) override
    {
        maskFrameStreamServer->NewFrame(result_value, width, height);
    }
    virtual void FrameOccupancyGrid(uchar3 *result_value, uint32_t width, uint32_t height) override
    {
        occupancyGridStreamServer->NewFrame(result_value, width, height);
    }
    virtual void FrameCaptured(SourceImageFormat *result_value, uint32_t width, uint32_t height) override
    {
        originalFrameStreamServer->NewFrame(result_value, width, height);
    }
    virtual void FrameSegmentation(SourceImageFormat *result_value, uint32_t width, uint32_t height) override
    {
        segmentedFrameStreamServer->NewFrame(result_value, width, height);
    }
};

ProcHandler *NewProcHandlerImplInstance(Logger *logger, const char *pubSubHost, int pubSubPort) { return new ProcHandlerImpl(logger, pubSubHost, pubSubPort); }