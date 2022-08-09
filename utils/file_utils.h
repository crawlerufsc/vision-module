#ifndef _FILE_UTILS_H
#define _FILE_UTILS_H

#include <string>
#include <fstream>
#include <sys/stat.h>

class FileUtils
{
public:
    static char *readFile(std::string filename)
    {
        std::ifstream file(filename, std::ifstream::binary);
        file.seekg(0, file.end);
        size_t length = static_cast<size_t>(file.tellg());
        file.seekg(0, file.beg);

        char *buffer = new char[length];
        file.read(buffer, length);
        file.close();
        return

            (char *)buffer;
    }

    static inline bool fileExists(const std::string &name)
    {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }
};

#endif