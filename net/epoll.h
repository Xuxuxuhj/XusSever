#pragma once
#include<vector>
#include"..//base/noncopyable.h"
class Channel;
class TimerManager;

class Epoll: noncopyable
{
public:
    Epoll();
    ~Epoll();
    void epoll_add(Channel* channel, int timeout);
    void epoll_del(Channel* channel);
    void epoll_mod(Channel* Channel, int timeout);
    std::vector<Channel*> poll();
private:
    std::vector<Channel*> getRetEvents(int num);
    static const int MAXFDS=100000;
    int efd_;
    std::vector<Channel*> events_;
    TimerManager* timeManager_;//only managed by Epoll
};