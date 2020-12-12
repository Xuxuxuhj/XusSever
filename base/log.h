#pragma once
#include"noncopyable.h"
#include"logstream.h"

enum LogType
{
    INFO=1,
    WARN,
    ERROR
};

class Logger:noncopyable
{
public:
    Logger(const char* fileName, int line, LogType logType=INFO);
    ~Logger();
    LogStream& stream()
    {
        return logStream_;
    }
    void output();
    void flush();
private:
    LogStream logStream_;
};

#define LOGERR Logger(__FILE__, __LINE__, ERROR).stream()
#define LOGWARN Logger(__FILE__, __LINE__, WARN).stream()
#define LOGINFO Logger(__FILE__, __LINE__, INFO).stream()
#define LOG Logger(__FILE__, __LINE__).stream()