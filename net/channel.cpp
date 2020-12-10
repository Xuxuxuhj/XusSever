#include"channel.h"
#include<sys/epoll.h>

Channel::Channel(int fd): fd_(fd), events_(0), lastEvents_(0){}

void Channel::handleEvents()
{
    if(retEvents_&EPOLLHUP&&!(retEvents_&EPOLLIN))
    {
        closeHandler_();
        return;
//shutdown
    }
    if(retEvents_&EPOLLIN||retEvents_&EPOLLPRI||retEvents_&EPOLLRDHUP)
    {
        readHandler_();
    }
    if(retEvents_&EPOLLOUT)
    {
        writeHandler_();
    }
    if(retEvents_&EPOLLERR)
    {
        errorHandler_();
    }
}