#include "../../utils/dropping_queue.h"
#include "../../utils/data_convert.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>
#include <string>

bool running = true;
DroppingQueue<char *> *queue;

void fastProducer(DroppingQueue<char *> *queue)
{
    int frame = 0;
    while (running)
    {
        std::stringstream ss;
        ss << "frame #" << frame;
        queue->add(DataConvert::convert(ss.str()));
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        frame++;
    }
}

void fastConsumerNoPop(DroppingQueue<char *> *queue)
{
    while (running)
    {
        char * frame = queue->peekNext();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //std::cout << "fast frame: " << frame << std::endl;
    }
}

void slowPopConsumer(DroppingQueue<char *> *queue)
{
    while (running)
    {
        char * frame = queue->pop();
        if (frame != 0)
            std::cout << "** SLOW data: " << frame << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}



int main (int argc, char **argv) {
    DroppingQueue<char *> *queue = new DroppingQueue<char *>();
    queue->deleteOnDrop();

    std::thread fastProducerThr(fastProducer, queue);
    std::thread fastConsumerNoPopThr(fastConsumerNoPop, queue);
    std::thread slowPopConsumerThr(slowPopConsumer, queue);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    running = false;

    fastProducerThr.join();
    fastConsumerNoPopThr.join();
    slowPopConsumerThr.join();

    return 0;
}