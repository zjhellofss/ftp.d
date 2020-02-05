//
// Created by fushenshen on 2020/2/4.
//

#ifndef EVENTLEARN_FTPLIST_H
#define EVENTLEARN_FTPLIST_H


#include "FTPTask.h"

class FTPList : public FTPTask
{
public:
    void parse(const std::string &type, const std::string &rawData) override;

    void read(bufferevent *bev) override;

    void write(bufferevent *bev) override;

    void event(bufferevent *bev, short what) override;

    std::string getFileInfo(const std::string &dir, const std::string &name);
};


#endif //EVENTLEARN_FTPLIST_H
