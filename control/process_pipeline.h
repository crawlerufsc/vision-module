#ifndef __PROCESS_PIPELINE_H
#define __PROCESS_PIPELINE_H

#include <queue>
#include <mutex>

template <typename T>
class ProcessPipeline
{
private:
    std::mutex *mtx;
    bool loop_run;
    T *frame;

    void captureThr()
    {
        while (this->loop_run)
        {
            T *f = this->captureNextFrame();
            if (f != NULL)
            {
                if (this->mtx->try_lock())
                {
                    if (this->frame != NULL)
                        delete this->frame;

                    this->frame = f;
                    this->mtx->unlock();
                }
            }
            transmitOriginal(f);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void processThr()
    {
        while (this->loop_run)
        {
            this->mtx->lock();
            if (this->frame != NULL)
            {
                process(this->frame);
            }
            this->mtx->unlock();
        }
    }

protected:
    virtual bool initialize() = 0;
    virtual T *captureNextFrame() = 0;
    virtual void transmitOriginal(T *) = 0;
    virtual void process(T *) = 0;
    virtual void onTerminate() = 0;

public:
    ProcessPipeline() {
        this->mtx = new std::mutex();
        loop_run = false;
        frame = NULL;
    }
    ~ProcessPipeline() {
        delete this->mtx;
    }
    void run()
    {
        initialize();

        loop_run = true;
        std::thread captureThread(&ProcessPipeline::captureThr, this);
        std::thread processThread(&ProcessPipeline::processThr, this);

        captureThread.join();
        processThread.join();
    }

    void terminate() {
        loop_run = false;
    }
};

#endif