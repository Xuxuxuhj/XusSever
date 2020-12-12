#pragma once
#include"noncopyable.h"
#include"mutex.h"
#include"condition.h"

class CountDownLatch:noncopyable
{
public:
    explicit CountDownLatch(int count);
    ~CountDownLatch();
    void wait();
    void countDown();
private:
    xu::mutex_t mutex_;
    xu::cond_t cond_;
    int count_;
};