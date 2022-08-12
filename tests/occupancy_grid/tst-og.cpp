#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>

#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <fstream>

#include <opencv2/opencv.hpp>
#include "../../occupancy_grid/occupancy_grid_impl.h"

using namespace std;
using namespace cv;
using namespace chrono;

#define DEBUG 1


char *convertToVector(Mat &m)
{
    cv::Size s = m.size();
    char *frame = (char *)malloc(sizeof(char) * (s.width * s.height + 1));
    char *p;
    int i, j;
    for (i = 0; i < s.height; ++i)
    {
        p = m.ptr<char>(i);
        for (j = 0; j < s.width; ++j)
        {
            frame[i * s.width + j] = p[j];
        }
    }
    return frame;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "use " << argv[0] <<" <file>\n";
        return 1;
    }

    
    Mat m = imread(argv[1], IMREAD_COLOR);
    cv::Size s = m.size();

    char *frame = convertToVector(m);

    OccupancyGridImpl<char> *compute = new OccupancyGridImpl<char>();

    char *og = compute->ComputeOcuppancyGrid((void *)frame, s.width, s.height);

    cv::Mat img = cv::Mat(cv::Size(compute->GetWidth(), compute->GetHeight()), CV_8UC1, og);
    std::vector<int> param(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 95;
    imwrite ("og.jpg", img, param);

    return 0;
}
