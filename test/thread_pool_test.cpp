#define THREAD_POOL_TEST
#include"..//base/thread_pool.h"
#include<iostream>

void func()
{
    std::cout<<"func in thread:"<<pthread_self()<<std::endl;
}


int main()
{
    for(int i=0;i<5;++i)
        threadPoolAdd(std::move(func));
}