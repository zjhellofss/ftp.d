//
// Created by fushenshen on 2020/2/4.
//

#include "FTPPort.h"
#include <vector>
#include <string>
#include <iostream>

//解析端口和ip
void FTPPort::parse(const std::string &type, const std::string &data)
{
    //msg :port 127,0,0,1,11,22
    std::vector<std::string> vals;
    std::string tmp;
    for (int i = 5; i < data.size(); ++i)
    {
        if (data[i] == ',' || data[i] == '\r')
        {
            vals.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += data[i];
    }
    if (vals.size() != 6)
    {
        this->sendMessageByServer("501 Syntax error in parameters or arguments.\r\n");
        return;
    }
    this->ip = vals[0] + "." + vals[1] + "." + vals[2] + "." + vals[3];
    this->port = atoi(vals[4].data()) * 256 + atoi(vals[5].data());
    this->ftpServer->port = this->port;
    this->ftpServer->ip = this->ip;
    this->sendMessageByServer("200 PORT command successful.\r\n");
}
