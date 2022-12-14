#ifndef __STREAM_SERVER_H
#define __STREAM_SERVER_H

#include <jetson-utils/videoOutput.h>
#include <string>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>

#include "../log/logger.h"
#include "../control/process_handler.h"
#include "../model/vision_formats.h"

#include <pubsub_client.h>

#define VISION_CMD_TOPIC "/vision-module/cmd"


using namespace std;

class ClientConnection
{
public:
    videoOutput *stream;
    const char *clientIP;
    int clientPort;

    ClientConnection(const char *clientIP, int clientPort, videoOutput *stream)
    {
        this->clientIP = clientIP;
        this->clientPort = clientPort;
        this->stream = stream;
    }

    ~ClientConnection()
    {
        delete stream;
        delete clientIP;
    }
};

class StreamServer : public PubSubClient
{
private:
    vector<ClientConnection *> *clients;
    Logger *logger;
    string serviceName;
    std::mutex *newFrameMtx;

    void OnStreaming(const char *clientIP, int clientPort);
    bool CheckOutputStreamExists(const char *clientIP, int clientPort);
    void CreateOutputStream(const char *clientIP, int clientPort);
    void RemoveOutputStream(const char *clientIP, int clientPort);
    // char * buildDefaultTopic(std::string serviceName);

protected:
    void onStop() override;
    void onReceived(std::string topic, std::string payload) override;

public:
    StreamServer(string serviceName, const char *topic, Logger *logger, const char *mqttHost, const int mqttPort);
    ~StreamServer();

    // void Start(int timeout);
    void NewFrame(SourceImageFormat *frame, uint32_t width, uint32_t height);
    void NewFrame(char *frame, uint32_t width, uint32_t height);
};

#endif