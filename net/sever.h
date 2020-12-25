#pragma once 
#include<functional>
#include<set>
#include"connection.h"
#include"..//base/noncopyable.h"
#include"..//base/smart_ptr.h"
#include"channel.h"

class Sever:noncopyable
{
public:
    typedef std::function<void()> Functor;
    explicit Sever(int port);
    ~Sever();
    void setConnReadHandler(Functor&& func);
    void setConnWriteHandler(Functor&& func);
    void setConnErrorHandler(Functor&& func);
    void handleListenRead();
private:
    int listenFd_;
    Channel listenChannel_; 
    Functor connReadHandler_;
    Functor connWriteHandler_;
    Functor connErrorHandler_;
    std::set<xu::shared_ptr<Connection>> connSet_;
};





