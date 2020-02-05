//
// Created by fushenshen on 2020/2/4.
//

#include "FTPUser.h"
#include <iostream>

void FTPUser::parse(const std::string &type, const std::string &cmd)
{
    using namespace std;
    cout << "FTPUser parse" << endl;
    this->sendMessageByServer("230 Login successful.\r\n");
}
