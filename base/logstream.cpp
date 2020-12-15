#include"logstream.h"


LogStream::LogStream(){}
LogStream::~LogStream(){}

LogStream& LogStream::operator<<(std::string &str)
{
    if(buffer_.avail()>str.size())
        buffer_.append(str.c_str(), str.size());
}
LogStream& LogStream::operator<<(const char* str)
{
     if(buffer_.avail()>strlen(str))
        buffer_.append(str, strlen(str));
}
LogStream& LogStream::operator<<(char c)
{
    if(buffer_.avail()>1)
        buffer_.append(&c, 1);
}

LogStream& LogStream::operator<<(int num)
{
    if(buffer_.avail()>STR_LEN_MAX)
        snprintf(buffer_.current(), STR_LEN_MAX, "%d", num);
}
LogStream& LogStream::operator<<(long long num)
{
    if(buffer_.avail()>STR_LEN_MAX)
        snprintf(buffer_.current(), STR_LEN_MAX, "%lld", num);
}
LogStream& LogStream::operator<<(short num)
{
    if(buffer_.avail()>STR_LEN_MAX)
        snprintf(buffer_.current(), STR_LEN_MAX, "%d", num);
}
LogStream& LogStream::operator<<(double num)
{
    if(buffer_.avail()>STR_LEN_MAX)
        snprintf(buffer_.current(), STR_LEN_MAX, "%f", num);
}