#pragma once
#include<vector>
#include"noncopyable.h"
#include"logstream.h"
#include"logfile.h"
#include"condition.h"
#include"smart_ptr.h"
#include"thread.h"
#include"mutex.h"
#include"countdownlatch.h"

class AsyncLog:noncopyable
{
public:
    typedef FixedBuffer<LARGE_BUFFER_SIZE> Buffer;
    AsyncLog();
    ~AsyncLog();
    void start();
    void append(const char* str, size_t len);
    void threadfunc(xu::shared_ptr<void> args=xu::shared_ptr<void>());
private:
    xu::shared_ptr<Buffer> curBuffer_;
    xu::shared_ptr<Buffer> nextBuffer_;
    std::vector<xu::shared_ptr<Buffer>> buffers_;
    xu::mutex_t mutex_;
    xu::cond_t cond_;
    Thread thread_;
    bool running_;
};