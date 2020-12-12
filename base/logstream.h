#pragma once
#include"noncopyable.h"
#include<string>
#include<string.h>

const int SMALL_BUFFER_SIZE=4096;
const int LARGE_BUFFER_SIZE=4096*100;

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
        return sizeof(data_)-size();
    }
    const char* data() const{
        return data_;
    }
    void reset()
    {
        cur_=data_;
    }
    char *current()
    {
        return cur_;
    }
    size_t size() const
    {
        return static_cast<size_t>(cur_-data_);
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
    LogStream& operator<<(const char* str);
    LogStream& operator<<(char c);
    LogStream& operator<<(int num);
    LogStream& operator<<(long long num);
    LogStream& operator<<(short num);
    LogStream& operator<<(double num);
    void reset()
    {
        buffer_.reset();
    }
    const Buffer& buffer() const
    {
        return buffer_;
    }
private:

    Buffer buffer_;
    const size_t STR_LEN_MAX=32;
};