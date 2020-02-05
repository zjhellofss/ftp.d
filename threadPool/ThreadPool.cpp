//
// Created by fushenshen on 2020/2/3.
//
#include "ThreadPool.h"
#include "Mthread.h"
#include "Task.h"
#include <thread>

ThreadPool *ThreadPool::pool = nullptr;

ThreadPool::ThreadPool()
{

}

void ThreadPool::init(int count)
{
    this->count = count;
    for (int i = 0; i < count; ++i)
    {
        auto *thread = new Mthread(i + 1);
        this->threads.push_back(thread);
        thread->start();
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(50ms);
    }
}

ThreadPool::~ThreadPool()
{
    if (!this->threads.empty())
    {
        for (auto d : this->threads)
        {
            delete d;
        }
    }
}

/**
 * 在线程池中分发任务
 * @param task 需要被线程池分发的task
 */
void ThreadPool::dispatch(Task *task)
{
    if (!task)
    {
        return;
    }
    int tid = (this->lastThread + 1) % this->count;
    //选中工作线程
    Mthread *t = this->threads[tid];
    this->lastThread += 1;
    t->addTask(task);
    //激活被选中的工作线程
    t->activate();
}

