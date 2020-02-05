//
// Created by fushenshen on 2020/2/4.
//

#ifndef EVENTLEARN_FTPPORT_H
#define EVENTLEARN_FTPPORT_H


#include "FTPTask.h"

class FTPPort : public FTPTask
{
public:
    void parse(const std::string &type, const std::string &data) override;
};


#endif //EVENTLEARN_FTPPORT_H
