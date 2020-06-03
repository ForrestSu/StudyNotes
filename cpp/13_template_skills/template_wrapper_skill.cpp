/**
 *  Author: sunquan
 *  Date: 2020-04-07 17:36:01
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory>
using namespace std;

template<typename T>
class Wrapper {
public:
    typedef void (T::*OnCB)(int mode);
    Wrapper(T* pObj, OnCB cb): m_obj_ptr(pObj), m_cb(cb){
    }
    void run_cb() {
        printf("m_cb = %p\n", m_cb);
        /**
         * 调用实例类,传入的回调函数; (回调函数是虚函数也支持)
         */
        (m_obj_ptr->*m_cb)(100);
    }
    ~Wrapper() {
        puts("free Wrapper!");
    }
private:
    T* m_obj_ptr;
    OnCB m_cb;
};

class Test {
public:
    Test() {
    }
    void init() {
       Wrapper<Test>* wrap = new Wrapper<Test>(this, &Test::call_back);
       wrap->run_cb();
       delete wrap;
    }
private:
     void call_back(int mode) {
        printf("call_back(), mode = %d\n", mode);
    }
};


int main() {
  Test t;
  t.init();
  return 0;
}

