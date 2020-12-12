#pragma once
#include"noncopyable.h"
#include"logstream.h"
#include"logfile.h"
#include"condition.h"
#include"countdownlatch.h"

class AsyncLog:noncopyable
{
public:
    typedef FixedBuffer<LARGE_BUFFER_SIZE> Buffer;
    AsyncLog();
    ~AsyncLog();
    void start();
    void append(const char* str, size_t len);
private:
    Buffer curBuffer_;
    Buffer nextBuffer_;
    vector<shared_ptr<Buffer>> buffers_;
    xu::mutex_t mutex_;
    xu::cond_t cond_;
    CountDownLatch latch_;
};