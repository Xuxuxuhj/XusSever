#include<sys/time.h>
#include"timer.h"

Timer::Timer(Channel* channel, int timeout): channel_(channel), isDeleted(false){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    limitTime_=tv.tv_sec*1000+tv.tv_usec;
}