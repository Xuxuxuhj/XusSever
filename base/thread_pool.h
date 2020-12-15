#pragma once
#include<functional>
#include<queue>
#include<list>
#include"noncopyable.h"
#include"mutex.h"
#include"condition.h"
#include"thread.h"

const int MIN_THR_SIZE=4;
const int MAX_THR_SIZE=20;
const int MAX_QUEUE_SIZE=20;

class ThreadPool
{
public:
    typedef std::function<void()>  Functor;
    explicit ThreadPool(int minThreadSize=MIN_THR_SIZE, int maxThreadSize=MAX_THR_SIZE, int maxQueueSize=MAX_QUEUE_SIZE);
    ~ThreadPool();
    void thread_add(Functor&& func);
    void thread_destroy();
    bool shutdown(){
        return shutdown_;
    }
private:
    void ajust_thread();
    void thread_func();
    Thread manager_;
    std::vector<xu::shared_ptr<Thread>> threadArray_;
    std::queue<Functor> funcQueue_;//避免queue无限增长
    xu::mutex_t mutex_;
    xu::cond_t notEmptyCond_;
    xu::cond_t fullCond_;//避免queue无限增长
    int busyNum_;
    int delNum_;
    int maxThreadNum_;
    int minThreadNum_;
    int maxQueueNum_;
    bool shutdown_;
    bool threadExit_;
};

void threadPoolAdd(ThreadPool::Functor&& func);