#pragma once
#include<sys/time.h>
#include<queue>
#include<vector>
#include"..//base/noncopyable.h"
#include"channel.h"

class Timer:noncopyable//owned by a connection
{
public:
    Timer(Channel* channel, int timeout);
    ~Timer();
    int getLimitTime(){
        return limitTime_;
    }
    bool setDelete()
    {
        isDeleted=true;
    }
private:
    Channel * channel_;
    int limitTime_;
    bool isDeleted;
};

class TimerCompareFunc{
public:
    bool operator()(Timer* a, Timer* b)
    {
        return a->getLimitTime()<b->getLimitTime();
    }
};

class TimerManager:noncopyable
{
public:
    TimerManager();
    ~TimerManager();
    void addTimer(Channel* request, int timeout);
    void delTimer(Channel* request);
private:
    void handleExpired();//cb of timerFd_ event
    std::priority_queue<Timer*, std::vector<Timer*>, TimerCompareFunc> timeQueue_;
    //Timer's lifetime is longer than connection, so use shared_ptr; 
    int timerFd_;
    Channel timerChannel;
};