#include"thread.h"
#include<unistd.h>
#include<sys/syscall.h>
#include<sys/types.h>
#include<pthread.h>
#include<string.h>
#include<sys/prctl.h>
#include<signal.h>


Thread::Thread(Functor &&func, const std::string& threadName):
    threadFunc_(func),
    threadName_(threadName),
    tid_(0),
    latch_(1),
    started_(false),
    joined_(false){}

Thread::~Thread()
{
    if(!joined_&&started_)
        pthread_detach(tid_);
}

struct ThreadData{
    ThreadData(Thread::Functor func, const std::string& name, CountDownLatch* platch, pthread_t* tid):
        func_(func),
        name_(name),
        platch_(platch),
        tid_(tid){
        }
    ~ThreadData(){}
    void init()
    {
        platch_->countDown();
        platch_->wait();
        assert(tid_!=0);
        CurrentThread::cacheTid_=*tid_;//tid() / pthread_self()
        CurrentThread::threadName_=name_.empty()?CurrentThread::threadName_:name_.c_str();
        prctl(PR_SET_NAME, CurrentThread::threadName_);
    }
    void runInThread()
    {
        init();
        func_();
    }
    Thread::Functor func_;
    CountDownLatch* platch_;
    std::string name_;
    pthread_t* tid_;
};

void* realThreadFunc(void* args)//真正给线程的函数
{   
    ThreadData* data=static_cast<ThreadData*>(args);//
    data->runInThread();
    delete data;
    return NULL;
}

void Thread::start()
{
    assert(!started_);
    started_=true;
    ThreadData* data(new ThreadData(threadFunc_, threadName_, &latch_, &tid_));
    if(pthread_create(&tid_, NULL, &realThreadFunc, data))
    {
        started_=false;
        delete data;
    }
    else
    {
        latch_.countDown();
        latch_.wait();
        assert(tid_>0);
    }
}
int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_=true;
    return pthread_join(tid_, NULL);
}

bool Thread::is_thread_alive()
{
    int ret = pthread_kill(tid_, 0);     //发0号信号，测试线程是否存活
    if (ret == ESRCH) {
        return false;
    }
    return true;
}

//////not used
int gettid()//pthread_t在不同进程间可能会重复，sys_gettid实质是读取thread的pid，不会重复
{
    return static_cast<int>(::syscall(SYS_gettid));
}

void CurrentThread::cacheTid()
{
    if(cacheTid_==0)
    {
        cacheTid_=gettid();
    }
}

