#include <pubsub_client.h>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <thread>

#define VISION_CMD_TOPIC ""

using nlohmann::json;

class StreamRequester : public PubSubClient {
public:    
    StreamRequester(char *ip) : PubSubClient(ip, 1883) {}
    virtual void onReceived(std::string topic, std::string payload) {}
    virtual void onStop() {}
};

void thrUdp1(){
    system("gst-launch-1.0 -v udpsrc port=20000 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! autovideosink");
}
void thrUdp2(){
    system("gst-launch-1.0 -v udpsrc port=20001 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! autovideosink");
}
void thrUdp3(){
    system("gst-launch-1.0 -v udpsrc port=20002 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! autovideosink");
}


int main (int argc, char **argv) {
    if (argc < 2) {
        printf ("use %s [local-ip]\n", argv[0]);
        exit(1);
    }

    StreamRequester requester("10.0.0.4");

    json j1 {
        {"ip", argv[1]},
        {"port", 20000},
        {"enable", true}
    };
    json j2 {
        {"ip", argv[1]},
        {"port", 20001},
        {"enable", true}
    };
    json j3 {
        {"ip", argv[1]},
        {"port", 20002},
        {"enable", true}
    };

    requester.publishTo("/vision-module/cmd/original", j1.dump());
    requester.publishTo("/vision-module/cmd/segmented", j2.dump());
    requester.publishTo("/vision-module/cmd/og", j3.dump());

    std::thread t1 (thrUdp1);
    std::thread t2 (thrUdp2);
    std::thread t3 (thrUdp3);

    t1.join();
    t2.join();
    t3.join();


   json j1d{
        {"ip", argv[2]},
        {"port", 20000},
        {"enable", false}
    };
    json j2d {
        {"ip", argv[2]},
        {"port", 20001},
        {"enable", false}
    };
    json j3d {
        {"ip", argv[2]},
        {"port", 20002},
        {"enable", false}
    };

    requester.publishTo("/vision-module/cmd/OriginalImageStream", j1d.dump());
    requester.publishTo("/vision-module/cmd/SegmentedFrameStream", j2d.dump());
    requester.publishTo("/vision-module/cmd/OGStream", j3d.dump());

    
}