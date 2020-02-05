//
// Created by fushenshen on 2020/2/3.
//

#ifndef EVENTLEARN_THREADPOOL_H
#define EVENTLEARN_THREADPOOL_H

#include <vector>
#include "Mthread.h"

class Task;

class ThreadPool
{
public:
    static ThreadPool *getPool()
    {
        if (!pool)
        {
            pool = new ThreadPool();
        }
        return pool;
    }

    void init(int count);

    void dispatch(Task *task);

    virtual ~ThreadPool();

private:
    ThreadPool();

private:
    int count = 0;
    int lastThread = -1;
    static ThreadPool *pool;
    std::vector<Mthread *> threads;
};

#endif //EVENTLEARN_THREADPOOL_H
