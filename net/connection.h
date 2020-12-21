#pragma once
#include"..//base/noncopyable.h"
#include<memory>

class Connection : noncopyable,
     public std::enable_shared_from_this<Connection>
{
public:
    explicit Connection();
    ~Connection();
    xu::weak_ptr<Timer> getTimer();
private:
    int fd_;
    xu::weak_ptr<Timer> timer_;//timer会先于conn析构
    Channel fdChannel_;
};