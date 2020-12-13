#include"asynclog.h"

AsyncLog::AsyncLog():curBuffer_(new Buffer), 
    nextBuffer_(new Buffer),
    mutex_(),
    cond_(mutex_),
    thread_(threadfunc, "log thread"),
    running_(false){
        curBuffer_->reset();
        nextBuffer_->reset();
        buffers_.reserve(16);
        thread_.start();
    }
AsyncLog::~AsyncLog()
{
    running_=false;
}

void AsyncLog::append(const char* str, size_t len)
{
    xu::lock_guard lg(mutex_);
    if(curBuffer_->avail()>len)
    {
        curBuffer_->append(str, len);
    }
    else
    {
        buffers_.push_back(move(curBuffer_));//after move curBuffer should be a default empty shared_ptr
        if(nextBuffer_==NULL)
        {
            nextBuffer_=new Buffer;
        }
        curBuffer_=std::move(nextBuffer_);
        curBuffer_->reset();
        cond_.notify();
    }
}

void AsyncLog::threadfunc(xu::shared_ptr<void> args)
{
    xu::shared_ptr<AsyncLog::Buffer> buffer1(new AsyncLog::Buffer);
    xu::shared_ptr<AsyncLog::Buffer> buffer2(new AsyncLog::Buffer);
    buffer1->reset();
    buffer2->reset();
    std::vector<xu::shared_ptr<Buffer>> buffersToWrite;
    buffersToWrite.reserve(16);
    LogFile output();
    running_=true;
    while(running_)
    {
        {
            xu::lock_guard lg(mutex_);
            if(buffers_.empty())
                cond_.waitSeconds(2);
            buffers_.push_back(std::move(curBuffer_));
            buffersToWrite.swap(buffers_);
            curBuffer_=std::move(buffer1);
            if(nextBuffer_==NULL)
                nextBuffer_=std::move(buffer2);
        }
        if(buffersToWrite.size()>25)// too much
            buffersToWrite.erase(buffersToWrite.begin()+25, buffersToWrite.end());
        for(auto &sp:buffersToWrite)
        {
            output->append(sp->data(), sp->size());
        }
        if(buffersToWrite.size()>2)
            buffersToWrite.resize(2);
        if(!buffer1)
        {
            assert(!buffersToWrite.empty());
            buffer1=buffersToWrite.back();
            buffersToWrite.pop_back();
        }
        if(!buffer2)
        {
            assert(!buffersToWrite.empty());
            buffer2=buffersToWrite.back();
            buffersToWrite.pop_back();
        }
    }
}