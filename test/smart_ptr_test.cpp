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
    return 0;
}