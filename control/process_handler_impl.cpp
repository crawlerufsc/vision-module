#include "process_handler.h"
#include "../communication/stream_server.h"

#include <jetson-utils/cudaMappedMemory.h>

#define MqttHost "10.0.0.4"
#define MqttPort 1883

class ProcHandlerImpl : public ProcHandler
{
    Logger *logger;
    StreamServer *originalFrameStreamServer;
    StreamServer *segmentedFrameStreamServer;
    StreamServer *maskFrameStreamServer;
    StreamServer *occupancyGridStreamServer;

private:
    void buildStreamServers()
    {
        originalFrameStreamServer = new StreamServer("OriginalImageStream", "/vision-module/cmd/original", logger, MqttHost, MqttPort);
        segmentedFrameStreamServer = new StreamServer("SegmentedFrameStream", "/vision-module/cmd/segmented", logger, MqttHost, MqttPort);
        maskFrameStreamServer = new StreamServer("MaskFrameStream", "/vision-module/cmd/mask", logger, MqttHost, MqttPort);
        occupancyGridStreamServer = new StreamServer("OGStream", "/vision-module/cmd/og", logger, MqttHost, MqttPort);

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
    ProcHandlerImpl(Logger *logger)
    {
        this->logger = logger;
        buildStreamServers();
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

ProcHandler *NewProcHandlerImplInstance(Logger *logger) { return new ProcHandlerImpl(logger); }