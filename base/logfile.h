#pragma once
#include<string>
#include"lock.h"
#include"noncopyable.h"

class LogFile:noncopyable
{
public:
    LogFile(const char* filename, int flushInterval);
    ~LogFile();
    void append();
private:
    
    xu::mutex_t mutex_;
}