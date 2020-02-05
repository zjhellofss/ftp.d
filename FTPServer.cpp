//
// Created by fushenshen on 2020/2/4.
//

#include "FTPServer.h"
#include <iostream>
#include <vector>
#include <event.h>
#include <event2/bufferevent.h>
#include <zconf.h>

using namespace std;


bool FTPServer::init()
{
    //设置ftp进入后的根目录
    this->currentPath = this->rootPath = "";
    //监听socket
    cout << "FTP Server" << endl;
    bufferevent *bev = bufferevent_socket_new(this->base, this->sock, BEV_OPT_CLOSE_ON_FREE);
    assert(bev != nullptr);
    this->setCallBack(bev);
    struct timeval timeval{300, 0};
    bufferevent_set_timeouts(bev, &timeval, &timeval);
    std::string helloMsg = "220 hello\r\n";
    bufferevent_write(bev, helloMsg.data(), helloMsg.size());
    return true;
}

void FTPServer::read(bufferevent *bev)
{
    const int length = 1024;
    char buf[length] = {0};
    std::string respMsg = "200 OK\r\n";
    this->bev = bev;
    while (true)
    {
        int len = bufferevent_read(bev, buf, length - 1);
        cout << buf << endl;
        if (len <= 0)
        {
            break;
        }
        std::string msg(buf);
        std::string data;
        for (char i : msg)
        {
            if (i != '\r' && i != ' ')
            {
                data += i;
            }
            else
            {
                break;
            }
        }
        auto iter = this->instructions.find(data);
        if (iter != this->instructions.end())
        {
            auto *task = dynamic_cast<FTPTask *>(iter->second);

            task->ftpServer = this;
            task->ip = ip;
            task->port = port;
            task->base = base;
            if (data == "PWD")
            {
                msg = this->currentPath;
            }
            task->parse(data, msg);

        }
        else
        {
            this->bev = bev;
            this->responseCommand("200 OK\r\n");
        }
    }
}

void FTPServer::write(bufferevent *bev)
{
    FTPTask::write(bev);
}

void FTPServer::event(bufferevent *bev, short what)
{

    if (what & (BEV_EVENT_TIMEOUT | BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
        cerr << "client error" << endl;
        if (bev)
        {
            bufferevent_free(bev);
            bev = nullptr;
        }
        delete this;
    }
}

FTPServer::~FTPServer()
{

}




