#include <iostream>
#include <event2/event.h>
#include <event2/listener.h>
#include <unistd.h>
#include <event2/bufferevent.h>
#include "ThreadPool.h"
#include "Task.h"
#include "../FTPServer.h"
#include "../FTPFactory.h"

void eventCb(struct bufferevent *bev, short events, void *arg)
{


}

void writeCb(struct bufferevent *bev, void *arg)
{
}

void readCb(struct bufferevent *bev, void *arg)
{

}


void listenCb(struct evconnlistener *listener, evutil_socket_t sockFd, struct sockaddr *, int socklen, void *arg)
{
    auto *ftpServer = FTPFactory::get()->createTask();
    auto *base = static_cast<event_base *>(arg);
    ftpServer->setBase(base);
    ftpServer->setSock(sockFd);
    auto pool = ThreadPool::getPool();
    pool->dispatch(ftpServer);

}


void server()
{

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return;
    event_base *base = event_base_new();
    assert(base);
    evutil_socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock > 0);
    struct sockaddr_in sockaddrIn;
    memset(&sockaddrIn, '\0', sizeof(sockaddr_in));
    sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddrIn.sin_port = htons(9991);
    sockaddrIn.sin_family = AF_INET;
    auto listener = evconnlistener_new_bind(base, listenCb, base,
                                            LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 32,
                                            (const sockaddr *) &sockaddrIn, sizeof(struct sockaddr_in));
    std::cout << "ftp server listen" << std::endl;
    event_base_dispatch(base);
    evconnlistener_free(listener);
    event_base_free(base);
}

int main()
{
    auto pool = ThreadPool::getPool();
    pool->init(5);
    server();
    return 0;
}

