#include"logfile.h"
#include<string.h>

File::File(const char* filename){
    FD_=fopen(filename, "ae");
    setbuffer(FD_, buffer_, sizeof(buffer_));
}
File::~File(){
    fclose(FD_);
}
void File::append(const char* str, size_t len)
{
    size_t remain=len;
    size_t n;
    while(remain>0)
    {
        if((n=fwrite_unlocked(str, 1, remain, FD_))<0)
        {
            perror("fwrite error");
        }
        remain-=n;
    }
}
void File::flush()
{
    fflush(FD_);
}
    
LogFile::LogFile(char* filename, int flushInterval)
{
    if(filename==NULL)
    {
        time_t t;
        time(&t);
        filename=ctime(&t);
    }
    strcat(filename, ".log");
    fd_=new File(filename);
    flushInterval_=flushInterval;
    count_=0;
}
LogFile::~LogFile()
{
    delete fd_;
}
void LogFile::append(const char* str, size_t len, bool doFlush)//default flush every flushInterval
{
    xu::lock_guard lock(mutex_);
    append_unlock(str, len);
    count_++;
    if(doFlush||count_==flushInterval_)
    {
        flush();
        count_=0;
    }
}

void LogFile::flush()
{
    fd_->flush();
}
    
void LogFile::append_unlock(const char* str, size_t len)
{
    fd_->append(str, len);
}