#pragma once
#include"noncopyable.h"
#include<string>
#include<string.h>

const int SMALL_BUFFER_SIZE=4096;

template<int SIZE>
class FixedBuffer{
public:
    FixedBuffer():cur_(data_){}
    ~FixedBuffer(){}
    void append(const char *data, size_t len)
    {
        memcpy(cur_, data, len);
        cur_+=len;
    }
    size_t avail(){
        return sizeof(data_)-static_cast<size_t>(cur_-data_);
    }
    char *current()
    {
        return cur_;
    }
private:
    char data_[SIZE];
    char* cur_;
};

class LogStream:noncopyable
{
public:
    typedef FixedBuffer<SMALL_BUFFER_SIZE> Buffer;
    LogStream();
    ~LogStream();
    LogStream& operator<<(std::string &str);
    LogStream& operator<<(char* str);
    LogStream& operator<<(char c);
    LogStream& operator<<(int num);
    LogStream& operator<<(long long num);
    LogStream& operator<<(short num);
    LogStream& operator<<(double num);
private:

    Buffer buffer_;
    const size_t STR_LEN_MAX=32;
};