//
// Created by fushenshen on 2020/2/5.
//

#include "FTPRETR.h"
#include <iostream>
#include <event.h>
#include <fstream>

void FTPRETR::parse(const std::string &type, const std::string &cmd)
{
    //完成文件的下载操作
    //解析指令中的文件名
    std::string msg = cmd;
    int pos = msg.rfind(" ") + 1;
    std::string filename = msg.substr(pos, msg.size() - pos - 2);
    std::string &curPath = this->ftpServer->currentPath;
    if (curPath[curPath.size() - 1] != '/')
    {
        curPath += '/';
    }
    std::string path = curPath + filename;
    this->fp = fopen(path.data(), "rb");
    if (this->fp)
    {
        //发送文件
        this->connectToClient();
        //通知对方开始下载文件
        this->sendMessageByServer("150 File OK\r\n");
        std::ifstream t(path);
        //触发写入事件
        bufferevent_trigger(bev, EV_WRITE, 0);
    }
    else
    {
        std::cerr << "文件打开失败" << std::endl;
        std::string errorMessage = "450 file open failed\r\n";
        this->sendMessageByServer(errorMessage);
    }
}

FTPRETR::~FTPRETR()
{

}

void FTPRETR::write(bufferevent *bev)
{
    if (!this->fp)
    {
        return;
    }
    else
    {
        //传送文件的数据
        char buf[1024] = {0};
        int len = fread(buf, 1, sizeof(buf), fp);
        if (len <= 0)
        {
            fclose(this->fp);
            this->fp = nullptr;
            this->sendMessageByServer("226 Transfer complete\r\n");
            this->taskClose();
            return;
        }
        else
        {
            this->responseCommand(buf);
        }

    }
}

void FTPRETR::event(bufferevent *bev, short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        std::cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << std::endl;
        this->taskClose();
        if (this->fp)
        {
            fclose(this->fp);
            this->fp = nullptr;
        }
    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        std::cout << "BEV_EVENT_CONNECTED" << std::endl;
    }
}
