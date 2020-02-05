//
// Created by fushenshen on 2020/2/5.
//

#include "FTPSTOR.h"
#include <iostream>
#include <event.h>

void FTPSTOR::parse(const std::string &type, const std::string &cmd)
{
    std::string msg = cmd;
    int pos = msg.rfind(" ") + 1;
    std::string filename = msg.substr(pos, msg.size() - pos - 2);
    std::string &curPath = this->ftpServer->currentPath;
    if (curPath[curPath.size() - 1] != '/')
    {
        curPath += '/';
    }
    std::string path = curPath + filename;
    this->fp = fopen(path.data(), "wb");
    if (this->fp)
    {
        this->connectToClient();
        //通知对方开始接收文件
        this->sendMessageByServer("125 File OK\r\n");
        //触发读取事件
        bufferevent_trigger(bev, EV_READ, 0);
    }
    else
    {
        std::cerr << "文件打开失败" << std::endl;
        std::string errorMessage = "450 file open failed\r\n";
        this->sendMessageByServer(errorMessage);
    }
}

void FTPSTOR::write(bufferevent *bev)
{
    FTPTask::write(bev);
}

void FTPSTOR::event(bufferevent *bev, short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        this->taskClose();
        if (this->fp)
        {
            fclose(this->fp);
            this->fp = nullptr;
        }
        this->sendMessageByServer("226 Transfer complete\r\n");
        std::cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << std::endl;

    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        std::cout << "BEV_EVENT_CONNECTED" << std::endl;
    }
}

void FTPSTOR::regInstruction(const std::string &string, Task *task)
{
    FTPTask::regInstruction(string, task);
}

void FTPSTOR::read(bufferevent *bev)
{
    if (!this->fp)
    {
        return;
    }
    for (;;)
    {
        const int length = 1024;
        char buf[length];
        int len = bufferevent_read(bev, buf, length);
        if (len <= 0)
        {
            return;
        }
        int size = fwrite(buf, 1, len, this->fp);
        if (size < 0)
        {
            break;
        }
    }
}
