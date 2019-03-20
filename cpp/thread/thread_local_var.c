#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;


/*
* g++ thread_local_var.c  -lpthread -Wall -O2  -o thread_local
*/

//是否添加 static 两种方式效果一样
__thread int var = 5;
static __thread int var2 = 15;

static void worker1(int arg)
{
    // 输出 6
    cout << "worker1 var :" << ++var << " address: " << &var << endl;
    cout << "worker1 var2 :" << ++var2 << " address: " << &var2 << endl;
}

static void worker2(int arg)
{
    // 等待线程1改变var值，验证是否影响线程2
    usleep(1000);
    cout << "worker2 var :" << --var << " address: " << &var << endl;
    cout << "worker2 var2 :" << --var2 << " address: " << &var2 << endl;
}

int test_local_var()
{
    // 修饰函数内的static变量
    static __thread int tmp_var = 10;

    std::thread th = std::thread(worker1, 1);
    std::thread th2 = std::thread(worker2, 2);

    th.join();
    th2.join();
    cout << endl;
    cout << "main var = " << var << " address :" << &var << endl;
    cout << "main var2 = " << var2 << " address :" << &var2 << endl;
    cout << "local tmp_var : "<< tmp_var << endl;
    return 0;
}

int main()
{
    test_local_var();
    return 0;
}

/*
worker1 var :6 address: 0xe1ac9c
worker1 var2 :16 address: 0xe1ab9c
worker2 var :4 address: 0xe1ab2c
worker2 var2 :14 address: 0xe1abac

main var = 5 address :0xe1abfc
main var2 = 15 address :0xe1ac2c
local tmp_var : 10
*/
