#include<sys/epoll.h>
#include<unistd.h>
#include"..//base//log.h"
#include"epoll.h"
#include"timer.h"

const int MAX_EVENT_NUM=4096;

Epoll::Epoll()
{
    efd_=epoll_create(MAXFDS);
    events_=std::vector<Channel*>(MAX_EVENT_NUM);
    timeManager_=new TimerManager;
}

Epoll::~Epoll()
{
    close(efd_);
    delete timeManager_;
}

void Epoll::epoll_add(Channel* request, int timeout)
{
    timeManager_->addTimer(request, timeout);
    struct epoll_event ev;
    ev.data.ptr=request;
    ev.events=request->getEvents();
    if(epoll_ctl(efd_, EPOLL_CTL_ADD, request->getFd(), &ev)!=0)
    {
        LOG<<"error epoll_add\n";
        return;
    }
    else
        return;

}

void Epoll::epoll_del(Channel* request)
{
    timeManager_->delTimer(request);
    struct epoll_event ev;
    ev.data.ptr=request;
    ev.events=request->getEvents();
    if(epoll_ctl(efd_, EPOLL_CTL_DEL, request->getFd(), &ev)!=0)
    {
        LOG<<"error epoll_del\n";
        return;
    }
    else
        return;
}

void Epoll::epoll_mod(Channel* request, int timeout)
{
    timeManager_->addTimer(request, timeout);
    if(!request->isSameEventsAsLast())
    {
        struct epoll_event ev;
        ev.data.ptr=request;
        ev.events=request->getEvents();
        if(epoll_ctl(efd_, EPOLL_CTL_MOD, request->getFd(), &ev)!=0)
        {
            LOG<<"error epoll_add\n";
            return;
        }
        else
            return;
    }
}

std::vector<Channel*> poll()
{

}

std::vector<Channel*> getRetEvents(int num)
{
    
}