//
// Created by fushenshen on 2020/2/5.
//

#ifndef EVENTLEARN_FTPSTOR_H
#define EVENTLEARN_FTPSTOR_H

#include "FTPTask.h"

class FTPSTOR : public FTPTask
{

public:
    void parse(const std::string &type, const std::string &cmd) override;

    void write(bufferevent *bev) override;

    void event(bufferevent *bev, short what) override;

    void regInstruction(const std::string &string, Task *task) override;

    void read(bufferevent *bev) override;

    FILE *fp = nullptr;

};

#endif //EVENTLEARN_FTPSTOR_H
