//
// Created by fushenshen on 2020/2/3.
//

#ifndef EVENTLEARN_MTHREAD_H
#define EVENTLEARN_MTHREAD_H

#include <event.h>
#include <deque>
#include <mutex>
#include "Task.h"

class Mthread
{
public:
    //start a thread
    void start();

    void loop();

    Mthread(int id);

    Mthread();

    bool setup();

    virtual ~Mthread();

    void notify(int fd, short which);

    void activate();

    void addTask(Task *);

private:
    int id;
    int nfd = 0;
    event_base *base = nullptr;
    std::deque<Task *> tasks;//任务队列
    std::mutex mutex4Task;

};


#endif //EVENTLEARN_MTHREAD_H
