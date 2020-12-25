#include"eventloop.h"
#include<unistd.h>
#include<sys/eventfd.h>

int create_eventfd()
{
    int fd=eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC);
    if(fd<0)
    {
        perror("eventfd error");
    }
    return fd;
}

EventLoop::EventLoop():
    eventfd_(create_eventfd()),
    tid_(CurrentThread::tid()),
    eventChannel_(eventfd_),
    poller_(),
    mutex_(),
    pendingFunctors_(),
    shutDown_(false),
    callingPendingFunctor_(false)
{
    eventChannel_.setReadHandler(std::bind(&EventLoop::handleEvents, this));
    eventChannel_.setEvents(EPOLLIN|EPOLLET);
    poller_.epoll_add(&eventChannel_, 0);
}

EventLoop::~EventLoop()
{
    close(eventfd_);
}

void EventLoop::loop()
{
    while(!shutDown_)
    {
        std::vector<Channel*> channels=poller_.poll();
        for(auto &channel:channels)
            channel->handleEvents();
    }
}
void EventLoop::runInLoop(std::function<void()> &&cb)
{
    if(isInLoopThread())
        cb();
    else
    {
        queueInLoop(std::move(cb));
    }    
}

void EventLoop::queueInLoop(std::function<void()> &&cb)
{
    {
        xu::lock_guard lg(mutex_);
        pendingFunctors_.push_back(cb);
    }
    if(!isInLoopThread()||callingPendingFunctor_)
    {
        wakeup();
    }
}

void EventLoop::wakeup()
{
    //write sth. to eventfd
    uint64_t one=1;
    if(write(eventfd_, &one, sizeof(one))<0)
    {
        perror("eventfd write error");
    }
    return;
}

void EventLoop::addToPoller(Channel* request, int timeout)
{
    poller_.epoll_add(request, timeout);
}

void EventLoop::removeFromPoller(Channel* request)
{
    poller_.epoll_del(request);
}

void EventLoop::modifyPoller(Channel* request, int timeout)
{
    poller_.epoll_mod(request, timeout);
}

void EventLoop::handleEvents()//doing pending functors;
{
    //TO DO
    uint64_t one;
    if(read(eventfd_, &one, sizeof(one))<0)
    {
        perror("read eventfd error");
    }
    std::vector<std::function<void()>> functorsToDo;
    {
        xu::lock_guard lg(mutex_);
        functorsToDo.swap(pendingFunctors_);
    }

    callingPendingFunctor_=true;
    for(auto &func:functorsToDo)
        func();
    
    callingPendingFunctor_=false;
}

void EventLoop::quit()//only do by manager of enentloop thread
{
    shutDown_=true;
    wakeup();
}