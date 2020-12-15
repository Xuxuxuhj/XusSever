#pragma once
#include<sys/time.h>
#include<queue>
#include<vector>
#include"..//base/noncopyable.h"
#include"channel.h"

class Timer:noncopyable
{
public:
    Timer();
    ~Timer();
    int getLimitTime(){
        return limitTime_;
    }
private:
    Channel * channel_;
    int limitTime_;
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
    std::priority_queue<Timer*, std::vector<Timer*>, TimerCompareFunc> timeQueue_;
};