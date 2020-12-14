#pragma once
#include<functional>
#include<string>
#include"noncopyable.h"
#include"countdownlatch.h"
#include"smart_ptr.h"

class Thread:noncopyable
{
public:
    typedef std::function<void()>  Functor;
    explicit Thread(Functor &&func, const std::string& threadName="");
    ~Thread();
    void start();
    int join();
    bool running() const {  return running_;}
    pid_t tid() const { return tid_;}
    const std::string& name() const {return threadName_;}
private:
    Functor threadFunc_;
    std::string threadName_;
    CountDownLatch latch_;
    pthread_t tid_;
    bool running_;
    bool joined_;
};

namespace CurrentThread{
__thread int cacheTid_=0;
__thread const char* threadName_="default";
void cacheTid();
inline int tid()
{
    if(__builtin_expect(cacheTid_==0, 0))
        cacheTid();
    return cacheTid_;
}
inline const char* tName()
{
    return threadName_;
}
}
