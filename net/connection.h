#pragma once
#include"..//base/noncopyable.h"
#include<memory>
#include"channel.h"
#include"timer.h"
#include"..//base/smart_ptr.h"

class Connection : noncopyable,
     public std::enable_shared_from_this<Connection>
{
public:
    explicit Connection(EventLoop* loop, int fd);
    ~Connection();
    xu::weak_ptr<Timer> getTimer();
    void shutDown();
    Channel& getSettingChannel()
    {
        return fdChannel_;
    }
private:
    EventLoop* loop_;
    int fd_;
    xu::weak_ptr<Timer> timer_;//timer会先于conn析构
    Channel fdChannel_;
};