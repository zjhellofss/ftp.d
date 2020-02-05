//
// Created by fushenshen on 2020/2/4.
//

#ifndef EVENTLEARN_FTPTASK_H
#define EVENTLEARN_FTPTASK_H

#include "threadPool/Task.h"
#include <event2/bufferevent.h>
#include <string>
#include <map>

class FTPTask : public Task
{
public:
    bool init() override;

    void responseCommand(const std::string &resp);


    virtual void parse(const std::string &type, const std::string &cmd);

    virtual ~FTPTask();

    virtual void read(bufferevent *bev)
    {

    }

    virtual void write(bufferevent *bev)
    {

    }

    virtual void event(bufferevent *bev, short what)
    {

    }

    void setCallBack(struct bufferevent *bev);

    static void callback(bufferevent *bev, void *arg);

    static void eventCallback(bufferevent *bev, short which, void *arg);

    static void writeCallback(bufferevent *bev, void *arg);

    virtual void regInstruction(const std::string &, Task *);

    void connectToClient();

    std::map<std::string, Task *> instructions;

    bufferevent *bev = nullptr;

    void setBev(bufferevent *bev);

    void taskClose();

    std::string getServerCurPath();

    void sendMessageByServer(const std::string &data);

public:
    int port = -1;
    FTPTask *ftpServer;
    std::string ip;
    std::string rootPath;
    std::string currentPath;
};


#endif //EVENTLEARN_FTPTASK_H
