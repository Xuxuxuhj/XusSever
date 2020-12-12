#include"logfile.h"


File::File(const char* filename){
    FD_=fopen(filename, "ae");
}
File::~File(){
    fclose(FD_);
}
void File::append(const char* str, size_t len)
{

}
    void flush();
    LogFile(const char* filename, int flushInterval);
    ~LogFile();
    void append(bool flush=false);//default flush every flushInterval
private:
    void flush();
    void append_unlock();