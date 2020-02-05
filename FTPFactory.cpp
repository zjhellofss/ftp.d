//
// Created by fushenshen on 2020/2/4.
//

#include "FTPFactory.h"
#include "FTPTask.h"
#include "FTPServer.h"
#include "FTPServer.h"
#include "FTPUser.h"
#include "FTPList.h"
#include "FTPPort.h"
#include "FTPRETR.h"
#include "FTPSTOR.h"

FTPFactory::~FTPFactory()
{


}

/**
 * 返回FTPServer的实例
 * @return
 */
Task *FTPFactory::createTask()
{

    auto *t = new FTPServer();
    t->regInstruction("USER", new FTPUser());
    t->regInstruction("PWD", new FTPList());
    t->regInstruction("LIST", new FTPList());
    t->regInstruction("CWD", new FTPList());
    t->regInstruction("PORT", new FTPPort());
    t->regInstruction("CDUP", new FTPList());
    t->regInstruction("RETR", new FTPRETR());
    t->regInstruction("STOR", new FTPSTOR());
    return t;
}

FTPFactory *FTPFactory::factory = nullptr;
