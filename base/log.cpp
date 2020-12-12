#include"log.h"
#include"asynclog.h"

static pthread_once_t once_init=PTHREAD_ONCE_INIT;
static AsyncLog* asyncLog_;

void initAsyncLog()
{
    asyncLog_=new AsyncLog;
    asyncLog_->start();
}


Logger::Logger(const char* fileName, int line, LogType logType)//TODO LogType
{
    logStream_<<fileName<<' '<<line<<' ';
    time_t t;
    time(&t);
    logStream_<<ctime(&t)<<'\n';
}

Logger::~Logger()
{
    output();
}

void Logger::output()
{
    pthread_once(&once_init, &initAsyncLog);
    LogStream::Buffer& buf(logStream_.buffer());
    asyncLog_->append(buf.data(), buf.size());
}
