#include<sys/time.h>
#include<sys/timerfd.h>
#include<unistd.h>
#include"timer.h"

int create_timerfd()
{
    int fd=timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
    if(fd<0)
    {
        perror("timerfd_create error");
    }
    return fd;
}

Timer::Timer(xu::shared_ptr<Connection>  conn, int timeout): conn_(conn), isDeleted(false)
{
    limitTime_=getTime(timeout);
}

int getTime(int timeout=0)//default is now
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec%10000)*1000+(tv.tv_usec)/1000+timeout;
}

Timer::~Timer(){}

TimerManager::TimerManager(EventLoop* loop):loop_(loop),timerQueue_(),timerFd_(create_timerfd()),
    timerChannel_(timerFd_)
{
    timerChannel_.setReadHandler(std::bind(&TimerManager::handleExpired, this));
    timerChannel_.setEvents(EPOLLIN|EPOLLONESHOT);
    loop->addToPoller(&timerChannel_, 0);//传递私有对象指针

}

void readTimerFd(int timerfd)
{
    uint64_t howmany;
    ssize_t n=read(timerfd, &howmany, sizeof(howmany));
    return;
}

void TimerManager::handleExpired()
{
    readTimerFd(timerFd_);
    int now=getTime();
    while(!timerQueue_.empty())
    {
        auto& top=timerQueue_.top();
        if(top->getDelete()||top->getLimitTime()<now)
        {
            timerQueue_.pop();
        }
        else
        {
            resetTimerFd();
            break;
        }
        
    }
}

void TimerManager::addTimer(xu::shared_ptr<Connection>  conn)
{
    xu::weak_ptr<Timer> weaktimer=conn->getTimer();
    assert(!weaktimer.expired());
    xu::shared_ptr<Timer> timer(weaktimer);
    auto& top=timerQueue_.top();
    if(top->getLimitTime()>timer->getLimitTime())
    {
        resetTimerFd();
    }
    timerQueue_.push(std::move(timer));
}

void delTimer(xu::shared_ptr<Connection>  conn)
{
    xu::weak_ptr<Timer> weaktimer=conn->getTimer();
    assert(!weaktimer.expired());
    xu::shared_ptr<Timer> timer(weaktimer);
    timer->setDelete();
}

void TimerManager::resetTimerFd()//TODO LimitTime not accurate
{
    struct timespec itInterval, itValue;
    struct itimerspec newValue;
    itInterval.tv_sec=0;
    itInterval.tv_nsec=0;
    auto& top=timerQueue_.top();
    itValue.tv_sec=top->getLimitTime()-getTime();
    itValue.tv_nsec=0;
    newValue.it_interval=itInterval;
    newValue.it_value=itValue;
    int ret=timerfd_settime(timerFd_, 0, &newValue, NULL);
    if(ret)
    {
        perror("timerfd_settime error");
        return;
    }
}