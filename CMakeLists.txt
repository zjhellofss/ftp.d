cmake_minimum_required(VERSION 3.15)
project(eventLearn)

set(CMAKE_CXX_STANDARD 14)
INCLUDE_DIRECTORIES(/usr/local/Cellar/libevent/2.1.8/include)
LINK_DIRECTORIES(/usr/local/Cellar/libevent/2.1.8/lib)
LINK_LIBRARIES(event event_pthreads)
link_directories(/usr/local/lib)
add_executable(eventLearn threadPool/ServerMain.cpp threadPool/ThreadPool.cpp threadPool/ThreadPool.h threadPool/Mthread.cpp threadPool/Mthread.h threadPool/Task.cpp threadPool/Task.h FTPServer.cpp FTPServer.h FTPTask.cpp FTPTask.cpp FTPTask.h FTPFactory.cpp FTPFactory.cpp FTPFactory.h FTPServer.cpp FTPServer.h FTPUser.cpp FTPUser.h FTPList.cpp FTPList.h FTPPort.cpp FTPPort.h FTPRETR.cpp FTPRETR.h FTPSTOR.cpp FTPSTOR.h)
