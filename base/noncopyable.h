#pragma once

class noncopyable{
public:
    noncopyable(){}
    ~noncopyable(){}
private:
    noncopyable(const noncopyable& another);
    noncopyable& operator=(const noncopyable& another);
};