#include "stream_server.h"

#include <nlohmann/json.hpp>
#include <jetson-utils/cudaMappedMemory.h>
#include <opencv2/core/cuda/vec_traits.hpp>
#include <sstream>

using nlohmann::json;

StreamServer::StreamServer(string serviceName, const char *topic, Logger *logger, const char *mqttHost, const int mqttPort) : PubSubClient(mqttHost, mqttPort, topic)
{
    this->serviceName = serviceName;
    this->logger = logger;
    this->clients = new vector<ClientConnection *>();
    this->newFrameMtx = new std::mutex();
}

StreamServer::~StreamServer()
{
    if (isConnected())
        Stop();

    onStop();

    delete this->clients;
    delete this->newFrameMtx;
}

void StreamServer::onStop()
{
    this->newFrameMtx->lock();

    if (this->clients->size() > 0)
    {

        for (ClientConnection *sc : *this->clients)
        {
            sc->stream->Close();
            delete sc;
        }

        this->clients->clear();
    }

    this->newFrameMtx->unlock();
}

void StreamServer::OnStreaming(const char *clientIP, int clientPort)
{
    logger->info("Streaming %s to %s:%d", serviceName.c_str(), clientIP, clientPort);
}

bool StreamServer::CheckOutputStreamExists(const char *clientIP, int clientPort)
{
    for (ClientConnection *sc : *this->clients)
    {
        if (strcmp(sc->clientIP, clientIP) == 0 && sc->clientPort == clientPort)
        {
            std::cout << "client exists: " << clientIP << ":" << clientPort << std::endl;
            return true;
        }
    }
    return false;
}
void StreamServer::CreateOutputStream(const char *clientIP, int clientPort)
{
    int len = 1024;
    char *uri = (char *)malloc(len);
    bzero(uri, 1024);
    snprintf(uri, len, "rtp://%s:%d", clientIP, clientPort);

    videoOptions options;
    options.width = 800;
    options.height = 600;
    options.zeroCopy = true;
    options.deviceType = videoOptions::DEVICE_FILE;
    options.ioType = videoOptions::OUTPUT;
    options.codec = videoOptions::CODEC_H264;
    ClientConnection *sc = new ClientConnection(clientIP, clientPort, videoOutput::Create(uri, options));

    this->newFrameMtx->lock();
    this->clients->push_back(sc);
    this->newFrameMtx->unlock();
}

void StreamServer::NewFrame(SourceImageFormat *frame, uint32_t width, uint32_t height)
{

    this->newFrameMtx->lock();
    if (this->clients->size() == 0)
    {
        this->newFrameMtx->unlock();
        return;
    }

    for (std::vector<ClientConnection *>::iterator itr = this->clients->begin(); itr != this->clients->end(); ++itr)
    {
        ClientConnection *sc = *itr;
        if (sc->stream == nullptr)
        {
            std::cout << "deleting streaming\n";
            this->clients->erase(itr);
            delete sc;
            continue;
        }

        sc->stream->Render(frame, width, height);

        char str[256];
        sprintf(str, "Video Viewer (%ux%u) | %.1f FPS", width, height, sc->stream->GetFrameRate());
        sc->stream->SetStatus(str);

        if (!sc->stream->IsStreaming())
        {
            sc->stream->Close();
            this->clients->erase(itr);
            delete sc;
        }
    }
    this->newFrameMtx->unlock();
}
void StreamServer::NewFrame(char *frame, uint32_t width, uint32_t height)
{
    this->newFrameMtx->lock();
    if (this->clients->size() == 0)
    {
        this->newFrameMtx->unlock();
        return;
    }

    for (std::vector<ClientConnection *>::iterator itr = this->clients->begin(); itr != this->clients->end(); ++itr)
    {
        ClientConnection *sc = *itr;
        if (sc->stream == nullptr)
        {
            std::cout << "deleting streaming\n";
            this->clients->erase(itr);
            delete sc;
            continue;
        }

        uint32_t n = width * height;
        uchar3 *p = (uchar3 *)malloc(sizeof(uchar3) * n);

        for (uint32_t i = 0; i < n; i++)
        {
            p[i].x = (uchar)frame[i];
            p[i].y = 0;
            p[i].z = 0;
        }

        sc->stream->Render(p, width, height);

        char str[256];
        sprintf(str, "Video Viewer (%ux%u) | %.1f FPS", width, height, sc->stream->GetFrameRate());
        sc->stream->SetStatus(str);

        if (!sc->stream->IsStreaming())
        {
            sc->stream->Close();
            this->clients->erase(itr);
            delete sc;
        }

        delete p;
    }
    this->newFrameMtx->unlock();
}

void StreamServer::RemoveOutputStream(const char *clientIP, int clientPort)
{
    int pos = 0;
    this->newFrameMtx->lock();
    for (auto it = this->clients->begin(); it != this->clients->end(); it++)
    {
        if (strcmp((*it)->clientIP, clientIP) == 0 && (*it)->clientPort == clientPort)
        {
            (*it)->stream->Close();
            this->clients->erase(it);
            this->newFrameMtx->unlock();
            return;
        }
        pos++;
    }
    this->newFrameMtx->unlock();
}

void StreamServer::onReceived(std::string topic, std::string payload)
{
    try
    {
        json s = json::parse(payload);

        std::string ip = s["ip"].get<std::string>();
        int port = s["port"].get<int>();
        bool add = s["enable"].get<bool>();

        printf("received stream request: %s, %d (%s)\n", ip.c_str(), port, add ? "add" : "del");

        if (add)
        {
            if (CheckOutputStreamExists(ip.c_str(), port))
            {
                logger->publishServingStream(ip.c_str(), port);
                return;
            }

            CreateOutputStream(ip.c_str(), port);
            logger->publishServingStream(ip.c_str(), port);
        }
        else
        {
            RemoveOutputStream(ip.c_str(), port);
        }
    }
    catch (nlohmann::json_abi_v3_11_2::detail::type_error &ex)
    {
        printf("invalid request for streaming: %s\n", payload.c_str());
    }
}