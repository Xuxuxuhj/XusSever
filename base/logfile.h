#pragma once
#include<string>
#include"mutex.h"
#include"noncopyable.h"

class File:noncopyable
{
public:
    File(const char* filename);
    ~File();
    void append(const char* str, size_t len);
    void flush();
private:
    FILE* FD_;
    char buffer_[4096*4];
};

class LogFile:noncopyable
{
public:
    LogFile(char* filename=NULL, int flushInterval=100);
    ~LogFile();
    void append(const char* str, size_t len, bool doFlush=false);//default flush every flushInterval
    void flush();
private:
    void append_unlock(const char* str, size_t len);
    File *fd_;
    int count_;
    int flushInterval_;
    xu::mutex_t mutex_;
};