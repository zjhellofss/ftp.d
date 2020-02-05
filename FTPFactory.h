//
// Created by fushenshen on 2020/2/4.
//

#ifndef EVENTLEARN_FTPFACTORY_H
#define EVENTLEARN_FTPFACTORY_H


#include "threadPool/Task.h"

class FTPFactory
{

public:
    virtual ~FTPFactory();

    static FTPFactory *get()
    {
        if (!factory)
        {
            factory = new FTPFactory();
        }
        return factory;
    }

    Task *createTask();

private:
    static FTPFactory *factory;
};


#endif //EVENTLEARN_FTPFACTORY_H
