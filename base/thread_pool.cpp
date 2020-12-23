#include"thread_pool.h"
#include<unistd.h>

#define DEFAULT_TIME 60*10

static ThreadPool* threadPool_=NULL;
static pthread_once_t once_init=PTHREAD_ONCE_INIT;

void initThreadPool()
{
    threadPool_=new ThreadPool();
}

void threadPoolAdd(ThreadPool::Functor&& func)
{
    pthread_once(&once_init, &initThreadPool);
    threadPool_->thread_add(std::move(func));
}

ThreadPool::ThreadPool(int minThreadSize, int maxThreadSize, int maxQueueSize):
    manager_(std::bind(&ThreadPool::ajust_thread, this), "threadpool manager"),
    threadArray_(),
    funcQueue_(),
    mutex_(),
    notEmptyCond_(mutex_),
    fullCond_(mutex_),
    busyNum_(0),
    delNum_(0),
    maxThreadNum_(maxThreadSize),
    minThreadNum_(minThreadSize),// not used
    maxQueueNum_(maxQueueSize),
    shutdown_(false),
    threadExit_(false){
    }

ThreadPool::~ThreadPool()
{
    {
        xu::lock_guard lg(mutex_);
        shutdown_=true;//manager_在下一个循环退出
        delNum_=threadArray_.size();
        notEmptyCond_.notifyAll();
    }
    //必须join前释放锁资源，否则所有thread拿不到锁
    //manager_退出前join所有thread
#ifdef THREAD_POOL_TEST
        std::cout<<"manager"<<manager_.tid()<<"joined"<<std::endl;
#endif
    manager_.join();
    //manager_不能在threadpool析构的时候自动detach，因为manager_还要访问threadpool成员
}

void ThreadPool::thread_add(Functor&& func)
{
    xu::lock_guard lg(mutex_);
    while(funcQueue_.size()==maxThreadNum_)
        fullCond_.wait();
    funcQueue_.push(func);
    notEmptyCond_.notifyAll();
}

void ThreadPool::thread_func()
{
    while(true)
    {
        Functor func;
        {
            xu::lock_guard lg(mutex_);
            --busyNum_;
            while(funcQueue_.empty()||delNum_==0)
                notEmptyCond_.wait();
            if(delNum_>0)
            {
                --delNum_;
                threadExit_=true;
                if(delNum_>0)
                    notEmptyCond_.notifyAll();//通知下一个退出
                break;//执行结束退出
                //pthread_exit(NULL);//ThreadData泄漏
            }
            func=funcQueue_.front();
            funcQueue_.pop();
            ++busyNum_;
            fullCond_.notifyAll();
        }
        func();                                                                                                       
    }
}

void ThreadPool::ajust_thread()
{
    for(int i=0;i<minThreadNum_;i++)
    {
        threadArray_.push_back(xu::shared_ptr<Thread>(new Thread(std::bind(&ThreadPool::thread_func, this), "thread threadpool")));
    }
    while(!threadPool_->shutdown())
    {
        sleep(DEFAULT_TIME);
        xu::lock_guard lg(mutex_);
        if(threadExit_)//有线程退出
        {
            std::vector<xu::shared_ptr<Thread>> newArray;
            for(auto &threadptr:threadArray_)
            {
                if(!threadptr->is_thread_alive())
                {
                    threadptr->join();
                }
                else{
                    newArray.push_back(std::move(threadptr));
                }
            }
            threadArray_.swap(newArray);
            threadExit_=false;
        }
        int threadNum=threadArray_.size();
        int busyNum=busyNum_;
        int taskNum=funcQueue_.size();
        if(threadNum<maxThreadNum_&&taskNum>(threadNum-busyNum))
        {
            int addthread=std::min(taskNum-(threadNum-busyNum), maxThreadNum_-threadNum);
            delNum_=0;
            for(int i=0;i<addthread;i++)
            {
                threadArray_.push_back(xu::shared_ptr<Thread>(new Thread(std::bind(&ThreadPool::thread_func, this), "threadpool thread")));
            }
        }
        else if(busyNum*2<threadNum)
        {
            delNum_=threadNum/2;//delNum_原本就不为0
        }
        //如何管理threadarray
    }
    //不需要加锁，因为只有他在管理
    for(auto &threadptr:threadArray_)
    {
#ifdef THREAD_POOL_TEST
        std::cout<<threadptr->tid()<<"joined"<<std::endl;
#endif
        threadptr->join();//如果不join会在自动在在threadpool析构的时候array析构后thread析构中detach，但detach后thread访问不了delNum_
    }
}