#include <iostream>
#include <string>
#include <stdarg.h>
#include <memory>
#include <stdexcept>


#include "logger.h"

using namespace std;

class DebugLogger : public Logger
{
public:
    void info(const char *format, ...) override
    {
        fprintf(stdout, "[INFO] ");
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        fprintf(stdout, "\n");
    };
    void warning(const char *format, ...) override
    {
        fprintf(stdout, "[WARNING] ");
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        fprintf(stdout, "\n");
    };
    void error(const char *format, ...) override
    {
        fprintf(stderr, "[ERROR] ");
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
        fprintf(stderr, "\n");
    };
    void critical(const char *format, ...) override
    {
        fprintf(stderr, "[ERROR] ");
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
        fprintf(stderr, "\n");
        exit(-1);
    };

    void publishCameraStatus(bool status) override
    {
        fprintf(stdout, "Camera status: %s", status ? "Active" : "Inactive");
    }
    void publishModuleStatus(bool status) override
    {
        fprintf(stdout, "Vision module status: %s", status ? "Active" : "Inactive");
    }
    void publishServingStream(const char *targetIP, int targetPort) override
    {

    }
};

Logger *NewDebugLoggerInstance() { return new DebugLogger(); }