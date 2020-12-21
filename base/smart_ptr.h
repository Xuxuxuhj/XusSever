#pragma once
#include<iostream>
#include<assert.h>
namespace xu{
    class _Counter{
    public:
        _Counter():shared_(0), weaked_(0){}
        ~_Counter(){}
        int shared_;
        int weaked_;
    };
    template<class T> class weak_ptr;
    template<class T>
    class shared_ptr{
    public:
        shared_ptr():cnt_(NULL), ptr_(NULL){}
        shared_ptr(T* ptr)
        {
            if(ptr==NULL)
            {
                cnt_=NULL;
                ptr_=NULL;
            }
            else
            {
                cnt_=new _Counter();
                ptr_=ptr;
                cnt_->shared_=1;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ shared initial"<<std::endl;
#endif
            }
        }
        shared_ptr(shared_ptr<T>& another){
            assert(another.ptr_!=NULL);
            cnt_=another.cnt_;
            ptr_=another.ptr_;
            cnt_->shared_++;
        }
        shared_ptr(shared_ptr<T>&& another){
            assert(this->ptr_==NULL);
            assert(another.ptr_!=NULL);
            this->cnt_=another.cnt_;
            this->ptr_=another.ptr_;
            another.cnt_=NULL;
            another.ptr_=NULL;
        }
        shared_ptr(weak_ptr<T>& weaked){
            assert(weaked.ptr_!=NULL);
            assert(weaked.cnt_->shared_>0);
            cnt_=weaked.cnt_;
            ptr_=weaked.ptr_;
            cnt_->shared_++;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ shared add"<<std::endl;
#endif
        }
        shared_ptr<T>& operator=(shared_ptr<T>& another)
        {
            assert(this->ptr_==NULL);
            assert(another.ptr_!=NULL);
            this->cnt_=another.cnt_;
            this->ptr_=another.ptr_;
            this->cnt_->shared_++;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ shared add"<<std::endl;
#endif
            return *this;
        }
        shared_ptr<T>& operator=(shared_ptr<T>&& another)
        {
            assert(this->ptr_==NULL);
            assert(another.ptr_!=NULL);
            this->cnt_=another.cnt_;
            this->ptr_=another.ptr_;
            another.cnt_=NULL;
            another.ptr_=NULL;
            //another.reset();
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ shared add"<<std::endl;
#endif
            return *this;
        }
        ~shared_ptr()
        {
            release();
        }
        operator bool()
        {
            return ptr_!=NULL;
        }
        bool operator==(const bool& type)
        {
            return bool(ptr_)==type;
        }
        bool operator==(const shared_ptr<T>& sptr)
        {
            if(ptr_!=this.sptr_)
                return true;
            else
                return false;
        }
        T& operator *() const
        {
            return *(ptr_);
        }
        T* operator ->() const
        {
            return ptr_;
        }
        void reset()
        {
            release();
            cnt_=NULL;
            ptr_=NULL;
        }
        friend class weak_ptr<T>;
    private:
        void release()
        {
            if(ptr_==NULL)
                return;
            cnt_->shared_--;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ shared decrease"<<std::endl;
#endif
            if(cnt_->shared_==0)
            {
                delete ptr_;
#ifdef TEST_SMART_PTR
    std::cout<<"ptr_ deleted"<<std::endl;
#endif
                if(cnt_->weaked_==0)
                {
                    delete cnt_;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ deleted"<<std::endl;
#endif
                }
            }
        }
        _Counter* cnt_;
        T* ptr_;
    };

    template<class T>
    class weak_ptr{
    public:
        weak_ptr():cnt_(NULL), ptr_(NULL){}
        weak_ptr(shared_ptr<T>& shared){
            cnt_=shared.cnt_;
            ptr_=shared.ptr_;
            cnt_->weaked_++;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ weaked add"<<std::endl;
#endif
        }
        weak_ptr(weak_ptr<T>& weaked){
            cnt_=weaked.cnt_;
            ptr_=weaked.ptr_;
            cnt_->weaked_++;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ weaked add"<<std::endl;
#endif
        }
        weak_ptr<T>& operator =(weak_ptr<T>& another)
        {
            assert(this->ptr_==NULL);
            this->cnt_=another.cnt_;
            this->cnt_=another.ptr_;
            this->cnt_->weaked_++;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ weaked add"<<std::endl;
#endif
            return *this;
        }
        weak_ptr<T>& operator =(shared_ptr<T>& another)
        {
            assert(this->ptr_==NULL);
            this->cnt_=another.cnt_;
            this->ptr_=another.ptr_;
            this->cnt_->weaked_++;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ weaked add"<<std::endl;
#endif
            return *this;
        }
        ~weak_ptr()
        {
            cnt_->weaked_--;
            if(cnt_->shared_==0&&cnt_->weaked_==0)
            {
                delete cnt_;
#ifdef TEST_SMART_PTR
    std::cout<<"cnt_ deleted"<<std::endl;
#endif
            }
        }
        bool expired()
        {
            if(cnt_->shared_>0)
                return true;
            else
                return false;
        }
    private:
        _Counter* cnt_;
        T* ptr_;
    };
}