#ifndef DROPPING_QUEUE_H
#define DROPPING_QUEUE_H

#include <queue>
#include <mutex>
#include <iostream>

template <typename T>
class DroppingQueue
{
private:
    T *data;
    std::mutex *mtx;
    int pos;
    int peek_pos;
    int size;

    void dropAll()
    {
        pos = -1;
        peek_pos = -1;
    }

public:
    DroppingQueue(int max)
    {
        data = (T *)malloc(sizeof(T) * (max + 1));
        mtx = new std::mutex();
        pos = 0;
    }
    ~DroppingQueue()
    {
        delete data;
        delete mtx;
    }


    void add(T val)
    {
        mtx->lock();
        pos++;
        data[pos] = val;
        mtx->unlock();
    }

    T peekNext()
    {
        T val = 0;
        mtx->lock();
        if (peek_pos < pos && pos >= 0)
        {
            val = data[++peek_pos];
        }
        mtx->unlock();
        return val;
    }

    T pop()
    {
        T val = 0;
        mtx->lock();

        if (pos >= 0)
        {
            val = data[pos];
        }

        dropAll();
        mtx->unlock();
        return val;
    }
};

#endif