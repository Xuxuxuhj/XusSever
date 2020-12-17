#pragma once
#include"..//base/noncopyable.h"
#include"..//base/mutex.h"
#include"channel.h"
#include"epoll.h"


class EventLoop:noncopyable
{
public:
    explicit EventLoop();
    ~EventLoop();
    void loop();
    void runInLoop();
    void addToPoller(Channel* request, int timeout);
    void removeFromPoller(Channel* request);
    void modifyPoller(Channel* request, int timeout);
private:
    int eventfd_;
    Channel eventChannel_;
    Epoll poller_;
    xu::mutex_t mutex_;
    std::vector<std::function<void()>> pendingFunctors_;
};