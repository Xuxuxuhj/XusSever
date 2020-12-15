#include<pthread.h>
#include<unistd.h>
#include<iostream>
#include<signal.h>

void* func(void* arg)
{
    std::cout<<"thread exit"<<std::endl;
    pthread_exit(NULL);
}

bool is_thread_alive(pthread_t tid_)
{
    int ret = pthread_kill(tid_, 0);     //发0号信号，测试线程是否存活
    if (ret == ESRCH) {
        return false;
    }
    return true;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, &func, NULL);
    sleep(1);
    if(is_thread_alive(tid))
    {
        std::cout<<"thread is alive"<<std::endl;
    }
    else
    {
        std::cout<<"thread is dead"<<std::endl;
    }
    if(pthread_join(tid, NULL)==0)
    {
        std::cout<<"join success"<<std::endl;
    }
    else
    {
        std::cout<<"join failed"<<std::endl;
    }
    
    return 0;
}