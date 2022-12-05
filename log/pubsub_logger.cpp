#include <iostream>
#include <string>
#include <stdarg.h>
#include <memory>
#include <stdexcept>
#include <vector>

#include "logger.h"
#include <pubsub_client.h>
#include <sstream>

#define _CRT_NO_VA_START_VALIDATION

using namespace std;


class PubSubLogger : public Logger, public PubSubClient
{
public:
    PubSubLogger(const char *mqttHost, const int mqttPort) : PubSubClient(mqttHost, mqttPort)
    {
    }

    void onReceived(std::string topic, std::string payload) override
    {
    }
    void onStop() override
    {
    }

    void info(const char *format, ...) override
    {
        va_list args;
        va_start(args, format);
        size_t len = std::vsnprintf(NULL, 0, format, args);
        va_end(args);
        std::vector<char> vec(len + 1);
        va_start(args, format);
        std::vsnprintf(&vec[0], len + 1, format, args);
        va_end(args);
        std::string msg = &vec[0];

        publishTo("/vision-module/log", "[INFO] " + msg);
    };
    void warning(const char *format, ...) override
    {
        va_list args;
        va_start(args, format);
        size_t len = std::vsnprintf(NULL, 0, format, args);
        va_end(args);
        std::vector<char> vec(len + 1);
        va_start(args, format);
        std::vsnprintf(&vec[0], len + 1, format, args);
        va_end(args);
        std::string msg = &vec[0];
        publishTo("/vision-module/log", "[WARNING] " + msg);
    };
    void error(const char *format, ...) override
    {
        va_list args;
        va_start(args, format);
        size_t len = std::vsnprintf(NULL, 0, format, args);
        va_end(args);
        std::vector<char> vec(len + 1);
        va_start(args, format);
        std::vsnprintf(&vec[0], len + 1, format, args);
        va_end(args);
        std::string msg = &vec[0];
        publishTo("/vision-module/log", "[ERROR] " + msg);
    };
    void critical(const char *format, ...) override
    {
        va_list args;
        va_start(args, format);
        size_t len = std::vsnprintf(NULL, 0, format, args);
        va_end(args);
        std::vector<char> vec(len + 1);
        va_start(args, format);
        std::vsnprintf(&vec[0], len + 1, format, args);
        va_end(args);
        std::string msg = &vec[0];
        publishTo("/vision-module/log", "[CRITICAL] " + msg);
    };

    void publishCameraStatus(bool status) override
    {
        publishTo("/vision-module/status/camera", status ? "true" : "false");
    }
    void publishModuleStatus(bool status) override
    {
        publishTo("/vision-module/status/module", status ? "true" : "false");
    }

    void publishServingStream(const char *targetIP, int targetPort) override
    {
        stringstream ss;
        ss << "{ \"targetIP\": \"" << targetIP << "\", \"targetPort\": " << targetPort << "}";
        // json resp{
        //     {"targetIP", targetIP},
        //     {"targetPort", targetPort}};

        publishTo("/vision-module/status/stream", ss.str());
    }
};

Logger *NewPubSubLoggerInstance(const char *mqttHost, int mqttPort) { return new PubSubLogger(mqttHost, mqttPort); }