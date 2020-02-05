//
// Created by fushenshen on 2020/2/4.
//

#ifndef EVENTLEARN_TASK_H
#define EVENTLEARN_TASK_H

class Task
{
public:
    struct event_base *base = nullptr;
    int sock = 0;
    int threadId = 0;

    virtual bool init() = 0;

    virtual ~Task();

    Task(event_base *base, int sock, int threadId);

    Task();

    event_base *getBase() const;

    void setBase(event_base *base);

    int getSock() const;

    void setSock(int sock);

    int getThreadId() const;

    void setThreadId(int threadId);
};

#endif //EVENTLEARN_TASK_H
