#include"connection.h"
#include"unistd.h"
#include<sys/socket.h>

const int DEFAULT_TIMEOUT=10;

Connection::Connection(EventLoop* loop, int fd):loop_(loop), fd_(fd), fdChannel_(fd_)
//构造函数不能用 shared_from_this
//shared_from_this:兼容xu::shared_ptr或者自己实现
{
    //
}

Connection::~Connection()
{
    close(fd_);
}

void Connection::shutDown()
{
    shutdown(fd_, SHUT_WR);
//关闭写端 发送FIN，可以被动，也可以主动，
//主动关闭：发送方shutdown write，对端read 0，就关闭写
}

