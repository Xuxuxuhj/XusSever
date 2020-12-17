#pragma once
#include<functional>
#include<sys/epoll.h>
#include"..//base/noncopyable.h"

class Channel: noncopyable
{//life time is same as connection
public:
    Channel(int fd);
    ~Channel(){}
    typedef std::function<void()> functor;
    void handleEvents();
    int getFd(){
        return fd_;
    }
    void setReadHandler(functor func)
    {
        readHandler_=func;
    }
    void setWriteHandler(functor func)
    {
        writeHandler_=func;
    }
    void setErrorHandler(functor func)
    {
        errorHandler_=func;
    }
    void setCloseHandler(functor func)
    {
        closeHandler_=func;
    }
    uint32_t getEvents()
    {
        return events_;
    }
    void setEvents(uint32_t events)
    {
        events_=events;
    }
    void setRetEvents(uint32_t events)
    {
        retEvents_=events;
    }
    bool isSameEventsAsLast()
    {
        if(events_&EPOLLONESHOT)
        {
            lastEvents_=events_;
            return false;
        }
        bool ret=events_==lastEvents_;
        lastEvents_=events_;
        return ret;
    }
private:
    int fd_;
    uint32_t events_;
    uint32_t retEvents_;
    uint32_t lastEvents_;
    functor readHandler_;
    functor writeHandler_;
    functor errorHandler_;
    functor closeHandler_;
};