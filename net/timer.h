#pragma once
#include<sys/time.h>
#include<queue>
#include<vector>
#include"..//base/noncopyable.h"
#include"..//base/smart_ptr.h"
#include"channel.h"
#include"connection.h"
#include"eventloop.h"

class Timer:noncopyable//owned by a connection
{
public:
    Timer(xu::shared_ptr<Connection> conn, int timeout);
    ~Timer();
    int getLimitTime(){
        return limitTime_;
    }
    bool getDelete()
    {
        return isDeleted;
    }
    void setDelete()
    {
        isDeleted=true;
    }
private:
    xu::shared_ptr<Connection> conn_;
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
    TimerManager(EventLoop* loop);
    ~TimerManager();
    void addTimer(xu::shared_ptr<Connection>  conn);
    void delTimer(xu::shared_ptr<Connection>  conn);
private:
    void handleExpired();//cb of timerFd_ event
    void resetTimerFd();
    EventLoop* loop_;
    std::priority_queue<xu::shared_ptr<Timer>, std::vector<xu::shared_ptr<Timer>>, TimerCompareFunc> timerQueue_;
    //Timer's lifetime is longer than connection, so use shared_ptr; 
    int timerFd_;
    Channel timerChannel_;
};