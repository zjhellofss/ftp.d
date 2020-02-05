//
// Created by fushenshen on 2020/2/5.
//

#ifndef EVENTLEARN_FTPRETR_H
#define EVENTLEARN_FTPRETR_H

#include "FTPTask.h"

class FTPRETR : public FTPTask
{

public:
    virtual ~FTPRETR();

    void parse(const std::string &type, const std::string &cmd) override;

    void write(bufferevent *bev) override;

    void event(bufferevent *bev, short what) override;

    FILE *fp = nullptr;
};

#endif //EVENTLEARN_FTPRETR_H
