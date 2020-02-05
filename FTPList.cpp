//
// Created by fushenshen on 2020/2/4.
//

#include "FTPList.h"
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string>
#include <dirent.h>

void FTPList::parse(const std::string &type, const std::string &rawData)
{
    using namespace std;
    std::string msg;
    cout << this->ftpServer->currentPath << endl;
    if (type == "CDUP")
    {
        std::string &curPath = this->ftpServer->currentPath;
        if (curPath.size() == 1 && curPath[0] == '/')
        {
            this->sendMessageByServer("250 Directory success changed\r\n");
        }
        else
        {
            if (curPath[curPath.size() - 1] == '/')
            {
                curPath = curPath.substr(0, curPath.size() - 1);
            }
            int pos = curPath.rfind("/");
            curPath = curPath.substr(0, pos);
            this->sendMessageByServer("250 Directory success changed\r\n");
        }

    }
    else if (type == "CWD")
    {
        int pos = rawData.rfind(" ") + 1;
        std::string path = rawData.substr(pos, rawData.size() - pos - 2);
        std::string &curPath = this->ftpServer->currentPath;
        if (path[0] == '/')
        {
            curPath = path;
        }
        else
        {
            if (curPath[curPath.size() - 1] != '/' && (!curPath.empty()) && path[0] != '/')
            {
                curPath += "/";
            }
            //CWD命令需要进入的路径
            curPath += path;
            if (curPath[curPath.size() - 1] != '/')
            {
                curPath += "/";
            }
        }

        this->sendMessageByServer("250 Directory success changed.\r\n");
    }
    else if (type == "PWD")
    {
        msg = "257 \"";
        msg += rawData;
        msg += "\" is current dir.\r\n";;
        cout << "current path :" << rawData << endl;
        this->sendMessageByServer(msg);
    }
    else if (type == "LIST")
    {
        std::string res;
        this->connectToClient();
        this->sendMessageByServer("150 Here comes the directory listing.\r\n");
        //回复消息
        std::string dir = this->getServerCurPath();
        //遍历文件夹
        char buf[512] = {0};
        char path[512] = {0};
        const char *dirname = dir.data();
        struct dirent **ptr = nullptr;
        int num = scandir(dirname, &ptr, nullptr, alphasort);
        if (num <= 0)
        {
            //log
        }
        else
        {
            for (int i = 0; i < num; ++i)
            {
                struct dirent *entry = ptr[i];
                char *name = entry->d_name;
                sprintf(path, "%s/%s", dirname, name);
                struct stat st{};
                int r = stat(path, &st);
                if (r != -1)
                {
                    std::string output = getFileInfo(path, name);
                    output += "\r\n";
                    res += output;
                }
                if (entry)
                    free(entry);
            }
        }
        if (ptr)
        {
            free(ptr);
        }
        this->responseCommand(res);
    }
}

void FTPList::read(bufferevent *bev)
{
    FTPTask::read(bev);
}

void FTPList::write(bufferevent *bev)
{
    this->sendMessageByServer("226 Transfer complete\r\n");
    this->taskClose();
}

void FTPList::event(bufferevent *bev, short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        std::cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << std::endl;
        this->taskClose();
    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        std::cout << "BEV_EVENT_CONNECTED" << std::endl;
    }
}

std::string FTPList::getFileInfo(const std::string &dir, const std::string &name)
{
    using std::string;
    string output;
    struct stat st;
    struct passwd *wd;
    struct group *gp;
    string tm;
    int ret = stat(dir.data(), &st);
    if (ret == -1)
    {
        return "";
    }
    if (S_ISREG(st.st_mode))
    {
        output += "-";
    }
    else if (S_ISDIR(st.st_mode))
    {
        output += "d";
    }
    else if (S_ISCHR(st.st_mode))
    {
        output += "c";
    }
    else if (S_ISBLK(st.st_mode))
    {
        output += "b";
    }
    else if (S_ISLNK(st.st_mode))
    {
        output += "l";
    }

    else if (S_ISFIFO(st.st_mode))
    {
        output += "f";
    }
    else if (S_ISSOCK(st.st_mode))
    {
        output += "s";
    }
    output += st.st_mode & S_IRUSR ? "r" : "-";
    output += st.st_mode & S_IWUSR ? "w" : "-";
    output += st.st_mode & S_IXUSR ? "x" : "-";
    output += st.st_mode & S_IRGRP ? "r" : "-";
    output += st.st_mode & S_IWGRP ? "w" : "-";
    output += st.st_mode & S_IXGRP ? "x" : "-";
    output += st.st_mode & S_IROTH ? "r" : "-";
    output += st.st_mode & S_IWOTH ? "w" : "-";
    output += st.st_mode & S_IXOTH ? "x" : "-";
    output += (" " + std::to_string(st.st_nlink));
    wd = getpwuid(st.st_uid);
    output += (" " + std::string(wd->pw_name));
    gp = getgrgid(st.st_gid);
    output += (" " + std::string(gp->gr_name));
    output += (" " + std::to_string(st.st_size));
    tm = ctime(&st.st_mtime);
    output += (" " + tm.substr(4, 12));
    output += (" " + name);
    return output;
}

