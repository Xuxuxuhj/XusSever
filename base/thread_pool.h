#pragma once
#include"noncopyable.h"

const int MIN_THR_SIZE=4;
const int MAX_THR_SIZE=20;
const int MAX_QUEUE_SIZE=20;

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();
    static void thread_create(int minThreadSize=MIN_THR_SIZE, int maxThreadSize=MAX_THR_SIZE, int maxQueueSize=MAX_QUEUE_SIZE);
    static 
}