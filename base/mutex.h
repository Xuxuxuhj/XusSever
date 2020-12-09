#pragma once
#include<pthread.h>
#include"noncopyable.h"

namespace xu{
    class mutex_t: noncopyable
    {
    public:
        mutex_t(){
            pthread_mutex_init(&mutex_, NULL);
        }
        ~mutex_t(){
            pthread_mutex_lock(&mutex_);
            pthread_mutex_destroy(&mutex_);
        }
        pthread_mutex_t* get(){
            return &mutex_;
        }
        void lock();
        void unlock();
    private:

        pthread_mutex_t mutex_;
    };

    class lock_guard: noncopyable
    {
    public:
        explicit lock_guard(mutex_t& mutex):mutex_(mutex){
            mutex_.lock();
        }
        ~lock_guard(){
            mutex_.unlock();
        }
    private:
        mutex_t& mutex_;
    };
}