//
// Created by fushenshen on 2020/2/3.
//

#include "Mthread.h"
#include <thread>
#include <iostream>
#include <evutil.h>
#include <zconf.h>
#include <event.h>

using std::cout;
using std::endl;
using std::cerr;

//线程被激活时的回调函数
static void eventCall(int fd0, short which, void *arg)
{
    auto *mthread = static_cast<Mthread *>(arg);
    mthread->notify(fd0, which);
}

void Mthread::start()
{
    //线程的初始化设置
    this->setup();
    std::thread th(&Mthread::loop, this);
    th.detach();
}

void Mthread::loop()
{
    assert(this->base);
    event_base_dispatch(this->base);
    event_base_free(this->base);
}

Mthread::Mthread(int id) : id(id)
{


}

Mthread::Mthread()
{


}
/**
 * 线程的初始化 初始化一条管道并在对端设置监听
 * @return
 */
bool Mthread::setup()
{
    int fd[2];
    int res = pipe(fd);
    if (res)
    {
        return false;
    }
    this->nfd = fd[1];
    auto config = event_config_new();
    event_config_set_flag(config, EVENT_BASE_FLAG_NOLOCK);
    auto b = event_base_new_with_config(config);
    assert(b);
    this->base = b;
    event *ev = event_new(base, fd[0], EV_PERSIST | EV_READ, eventCall, this);
    event_add(ev, 0);
    event_config_free(config);
    return true;
}

Mthread::~Mthread()
{
    if (this->base)
    {
        event_base_free(this->base);
        this->base = nullptr;
    }
}

//线程接收激活消息的过程
void Mthread::notify(int fd, short which)
{
    char buf[2] = {0};
    int len = read(fd, buf, 1);
    assert(len > 0);
    cout << id << " thread " << endl;
    //线程被激活
    this->mutex4Task.lock();
    if (this->tasks.empty())
    {
        this->mutex4Task.unlock();
        return;
    }
    //从Task队列中取出一个Task
    Task *task = this->tasks.front();
    this->tasks.pop_front();
    this->mutex4Task.unlock();
    task->init();
}

/**
 * 激活对应的线程
 */
void Mthread::activate()
{
    //通过管道向对端传送数据而对端正在以非阻塞的形式接收数据
    int res = write(this->nfd, "c", 1);
    if (res <= 0)
    {
        cerr << this->id << " thread activate error " << endl;
    }
}

/**
 * 向线程中增加task
 * @param task 需要增加的任务
 */
void Mthread::addTask(Task *task)
{
    if (!task)
    {
        return;
    }
    task->setBase(this->base);
    this->mutex4Task.lock();
    this->tasks.push_back(task);
    this->mutex4Task.unlock();
}



