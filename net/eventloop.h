#pragma once
#include"..//base/noncopyable.h"
#include"..//base/mutex.h"
#include"..//base/thread.h"
#include"channel.h"
#include"epoll.h"


class EventLoop:noncopyable
{
public:
    explicit EventLoop();
    ~EventLoop();
    bool isInLoopThread()
    {
        return tid_==CurrentThread::tid();
    }
    void loop();
    void runInLoop(std::function<void()>&& cb);
    void addToPoller(Channel* request, int timeout);
    void removeFromPoller(Channel* request);
    void modifyPoller(Channel* request, int timeout);
    void quit();
private:
    void handleEvents();
    void queueInLoop(std::function<void()>&& cb);
    void wakeup();
    int eventfd_;
    int tid_;
    Channel eventChannel_;
    Epoll poller_;
    xu::mutex_t mutex_;
    std::vector<std::function<void()>> pendingFunctors_;
    bool shutDown_;
    bool callingPendingFunctor_;
};