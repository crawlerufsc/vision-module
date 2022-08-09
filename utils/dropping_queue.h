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
    bool shouldDeleteWhenDropping;

    void dropAllExceptLast()
    {
        if (shouldDeleteWhenDropping)
        {
            if (pos > 0)
            {
                std::cout << "dropping all except #" << pos << "(" << data[pos] << ")\n";
                for (int i = 0; i < pos - 1; i++)
                    delete data[i];
            }
        }
        pos = -1;
        peek_pos = -1;
    }
    void increaseBufferSize()
    {
        std::cout << "buffer size increased to " << 2 * size << "\n";
        T *newdata = (T *)malloc(sizeof(T) * (2 * size + 1));

        for (int i = 0; i < size; i++)
            newdata[i] = data[i];

        delete this->data;
        this->data = newdata;
        this->size = 2 * size;
    }

    void initialize(int init_size)
    {
        shouldDeleteWhenDropping = false;
        size = init_size;
        data = (T *)malloc(sizeof(T) * (init_size + 1));
        mtx = new std::mutex();
        pos = 0;
    }

public:
    DroppingQueue(int init_size)
    {
        initialize(init_size);
    }
    DroppingQueue()
    {
        initialize(2);
    }
    ~DroppingQueue()
    {
        delete data;
        delete mtx;
    }
    void deleteOnDrop()
    {
        this->shouldDeleteWhenDropping = true;
    }

    void add(T val)
    {
        mtx->lock();
        pos++;
        if (pos == size)
        {
            increaseBufferSize();
        }
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
            dropAllExceptLast();
        }

        mtx->unlock();
        return val;
    }
};

#endif