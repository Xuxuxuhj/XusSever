#include"countdownlatch.h"

CountDownLatch::CountDownLatch(int count):mutex_(), cond_(mutex_), count_(count){}

CountDownLatch::~CountDownLatch(){}

void CountDownLatch::wait()
{
    xu::lock_guard lg(mutex_);
    while(count_>0)
        cond_.wait();
}
void CountDownLatch::countDown()
{
    xu::lock_guard lg(mutex_);
    count_--;
    if(count_==0)
        cond_.notifyAll();
}