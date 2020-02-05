//
// Created by fushenshen on 2020/2/4.
//

#include "FTPTask.h"
#include "FTPServer.h"
#include <event2/bufferevent.h>
#include <event.h>
#include <iostream>

bool FTPTask::init()
{
    return false;
}

void FTPTask::setCallBack(bufferevent *bev)
{
    bufferevent_setcb(bev, callback, writeCallback, eventCallback, this);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

/**
 * libevent的回调函数
 * @param bev
 * @param arg
 */
void FTPTask::callback(bufferevent *bev, void *arg)
{
    auto *ftpTask = static_cast<FTPServer *>(arg);
    ftpTask->read(bev);
}

void FTPTask::eventCallback(bufferevent *bev, short which, void *arg)
{
    auto *ftpTask = static_cast<FTPServer *>(arg);
    ftpTask->event(bev, which);
}

void FTPTask::writeCallback(bufferevent *bev, void *arg)
{
    auto *ftpTask = static_cast<FTPServer *>(arg);
    ftpTask->write(bev);
}

void FTPTask::regInstruction(const std::string &cmd, Task *task)
{
    if (!task || cmd.empty())
    {
        return;
    }
    auto iter = this->instructions.find(cmd);
    if (iter != this->instructions.end())
    {
        return;
    }
    this->instructions.insert({cmd, task});

}

FTPTask::~FTPTask()
{
    for (const auto &ite:this->instructions)
    {
        auto *d = (ite.second);
        if (d != nullptr)
        {
            delete d;
        }
    }
}

void FTPTask::parse(const std::string &type, const std::string &rawData)
{

}

void FTPTask::responseCommand(const std::string &resp)
{
    if (this->bev == nullptr)
    {
        return;
    }
    else
    {
        bufferevent_write(this->bev, resp.data(), resp.size());
    }
}


void FTPTask::setBev(bufferevent *bev)
{
    FTPTask::bev = bev;
}

void FTPTask::connectToClient()
{
    if (ip.empty() || port <= 0 || !base)
    {
        std::cerr << "ConnectPORT failed ip or port or base is null" << std::endl;
        return;
    }
    this->taskClose();
    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin{};
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    evutil_inet_pton(AF_INET, ip.c_str(), &sin.sin_addr.s_addr);
    this->setCallBack(bev);
    timeval rt = {60, 0};
    bufferevent_set_timeouts(bev, &rt, 0);
    bufferevent_socket_connect(bev, (sockaddr *) &sin, sizeof(sin));
}

void FTPTask::taskClose()
{
    if (bev)
    {
        bufferevent_free(this->bev);
        bev = nullptr;
    }
}

void FTPTask::sendMessageByServer(const std::string &data)
{
    if (this->ftpServer != nullptr)
    {
        this->ftpServer->responseCommand(data);
    }
}

std::string FTPTask::getServerCurPath()
{
    std::string serverPath = this->ftpServer->currentPath;
    if (serverPath.empty())
    {
        return "";
    }
    else
    {
        return serverPath;
    }
}
