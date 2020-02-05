//
// Created by fushenshen on 2020/2/4.
//

#ifndef EVENTLEARN_FTPUSER_H
#define EVENTLEARN_FTPUSER_H


#include "FTPTask.h"

class FTPUser : public FTPTask
{
public:
    void parse(const std::string &type, const std::string &cmd) override;
};


#endif //EVENTLEARN_FTPUSER_H
