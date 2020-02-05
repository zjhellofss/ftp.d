//
// Created by fushenshen on 2020/2/4.
//

#ifndef EVENTLEARN_FTPSERVER_H
#define EVENTLEARN_FTPSERVER_H

#include "FTPTask.h"
#include <string>
#include <map>

class FTPServer : public FTPTask
{
    bool init() override;

public:
    virtual ~FTPServer();

public:
    void read(bufferevent *bev) override;

    void write(bufferevent *bev) override;

    void event(bufferevent *bev, short what) override;


    std::map<std::string, Task *> instrucitons;

    //进入ftp服务器后的初试目录


};

#endif //EVENTLEARN_FTPSERVER_H
