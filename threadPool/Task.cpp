//
// Created by fushenshen on 2020/2/4.
//

#include "Task.h"

Task::Task(event_base *base, int sock, int threadId) : base(base), sock(sock), threadId(threadId)
{}

Task::Task()
{}

event_base *Task::getBase() const
{
    return base;
}

void Task::setBase(event_base *base)
{
    Task::base = base;
}

int Task::getSock() const
{
    return sock;
}

void Task::setSock(int sock)
{
    Task::sock = sock;
}

int Task::getThreadId() const
{
    return threadId;
}

void Task::setThreadId(int threadId)
{
    Task::threadId = threadId;
}

Task::~Task()
{

}
