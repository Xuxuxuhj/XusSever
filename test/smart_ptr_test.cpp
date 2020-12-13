#include<iostream>
#define TEST_SMART_PTR
#include"..//base//smart_ptr.h"
using namespace std;
int main()
{
    {
        xu::shared_ptr<int> p1;
        xu::shared_ptr<int> p2(new int);
        p1=p2;
    }
    cout<<"test part1 end"<<endl;
    {
        xu::weak_ptr<int> p3;
        {
            xu::shared_ptr<int> p1(new int);
            xu::weak_ptr<int> p2(p1);
            p3=p1;
        }
    }
    cout<<"test part2 end"<<endl;
    {
        xu::shared_ptr<int> p1;
        xu::shared_ptr<int> p2(new int(1));
        p1=std::move(p2);
        if(!p2)
            cout<<"p2 is empty()"<<endl;
        else
        {
            cout<<"p2 is not empty()"<<"p2 points to:"<<*p2<<endl;
        }
        if(!p1)
            cout<<"p1 is empty()"<<endl;
        else
        {
            cout<<"p1 is not empty()"<<"p1 points to:"<<*p1<<endl;;
        }
        cout<<"after p1 reset"<<endl;
        p1.reset();
        if(!p1)
            cout<<"p1 is empty()"<<endl;
        else
        {
            cout<<"p1 is not empty()"<<"p1 points to:"<<*p1<<endl;;
        }
    }
     cout<<"test part3 end"<<endl;
    return 0;
}