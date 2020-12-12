#pragma once
#include<time.h>
#include<errno.h>
#include"noncopyable.h"
#include"mutex.h"

namespace xu{
    class cond_t:noncopyable{
    public:
        explicit cond_t(mutex_t& mutex):mutex_(mutex){
            pthread_cond_init(&cond_, NULL);
        }
        ~cond_t(){
            pthread_cond_destroy(&cond_);
        }
        void wait()
        {
            pthread_cond_wait(&cond_, mutex_.get());
        }
        void notify()
        {
            pthread_cond_signal(&cond_);
        }
        void notifyAll()
        {
            pthread_cond_broadcast(&cond_);
        }
        bool waitSeconds(int seconds)
        {
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime);
            abstime.tv_sec+=static_cast<time_t>(seconds);
            return ETIMEDOUT==pthread_cond_timedwait(&cond_, mutex_.get(), &abstime);
        }
    private:
        mutex_t& mutex_;
        pthread_cond_t cond_;
    };
}