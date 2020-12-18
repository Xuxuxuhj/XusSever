#include<sys/epoll.h>
#include<unistd.h>
#include"..//base//log.h"
#include"epoll.h"
#include"timer.h"

const int MAX_EVENT_NUM=4096;
const int EPOLL_WAIT_TIME=10000;

Epoll::Epoll():efd_(epoll_create(MAXFDS)),
    events_(MAX_EVENT_NUM),
    timeManager_()
{}

Epoll::~Epoll()
{
    shutDown_=false;
    close(efd_);
}

void Epoll::epoll_add(Channel* request, int timeout)
{
    timeManager_.addTimer(request, timeout);
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
    timeManager_.delTimer(request);
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
    timeManager_.addTimer(request, timeout);
    if(!request->isSameEventsAsLast())//if is oneshot
//何时使用EPOLLONESHOT事件：即使我们使用ET模式，一个socket上的某个事件还是可能被触发
//多次。这在并发程序中就会引起一个问题：比如一个线程（或进程）在读取完某个socket上的数据
// 后开始处理这些数据，而在数据的处理过程中该socket上又有新数据可读（EPOLLIN再次被触发），
// 此时另外一个线程（或进程）被唤醒来读取这些新的数据。于是就出现了两个线程同时操作一个
// socket的局面。这当然不是我们期望的，我们期望的是一个socket连接在任一时刻都只被一个线程
// 处理。这一点可以使用epoll的EPOLLONESHOT事件实现
// EPOLLONESHOT事件的作用：对于注册了EPOLLONESHOT事件的文件描述符，操作系统最多触发
// 其注册的一个可读、可写或者异常事件，且只触发一次，在触发的时候，除非我们重新将这个文件
// 描述符封装为一个事件再次放入epoll事件池。这样，当一个线程在处理某个socket的时候，其他
// 线程是不可能有机会操作该socket的
// 因此，注册了EPOLLONESHOT事件的socket一旦被某个线程处理完毕，该线程就应该立即重置
// （epoll_ctl(,EPOLL_CTL_MOD,,)）这个socket的EPOLLONESHOT事件，以确保这个socket
// 下一次可读时，其EPOLLIN事件能被触发，进而让其他工作线程有机会继续处理这个socket

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

std::vector<Channel*> Epoll::poll()
{
    while(true)
    {
        int num=epoll_wait(efd_, &events_.front(), MAXFDS, EPOLL_WAIT_TIME);
        if(num<0)
            perror("epoll wait error:");
        else
        {
            std::vector<Channel*> req_data=getRetEvents(num);
            return req_data;
        }
        
    }
}

std::vector<Channel*> Epoll::getRetEvents(int num)
{
    std::vector<Channel*> channels;
    for(auto &ev:events_)
    {   
        Channel* channel=(Channel*)ev.data.ptr;
        channel->setRetEvents(ev.events);
        channels.push_back(channel);
    }
    return channels;
}