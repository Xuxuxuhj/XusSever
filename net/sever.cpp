#include"sever.h"
#include"unistd.h"
#include<sys/types.h>
#include<sys/socket.h>

int getListenFd(int port)
{
    //
    return 0;
}

Sever::Sever(int port):listenFd_(getListenFd(port)), listenChannel_(listenFd_)
{
    listenChannel_.setReadHandler(std::bind(&Sever::handleListenRead, this));
}

Sever::~Sever()
{
    close(listenFd_);
}

void Sever::setConnReadHandler(Functor&& func)
{
    connReadHandler_=func;
}

void Sever::setConnWriteHandler(Functor&& func)
{
    connWriteHandler_=func;
}

void Sever::setConnErrorHandler(Functor&& func)
{
    connErrorHandler_=func;
}

void Sever::handleListenRead()
{
    int newfd=0;
    while((newfd=accept(listenFd_, NULL, NULL))>0)
    {
        EventLoop* loop=EventLoopThreadPool->getNextLoop();
        xu::shared_ptr<Connection> conn(new Connection(loop, newfd));
        Channel& connChannel=conn->getSettingChannel();
        connChannel.setReadHandler(connReadHandler_);
        connChannel.setWriteHandler(connWriteHandler_);
        connChannel.setErrorHandler(connErrorHandler_);
        connSet_.insert(conn);
        
    }
}