
#include <string>
#include <iostream>
#include <future>
#include <unistd.h>

#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

using std::cout;
using std::endl;

// 测试async
void test_async()
{
    std::cout << "[0] main thread id: " << gettid() << endl;
    cout << "[1] begin"<< endl;
    // default launch policy
    std::future<int> f0 = std::async([]() {
        std::cout << "[A] thread id [async|deferred]: " << gettid() << endl;
        sleep(8);
        std::cout <<"[A] f0 exit!" <<endl;
        return 5;
    });
    cout << "[2] f0 create ok"<< endl;

    std::future<int> f1 = std::async(std::launch::async, []() {
        std::cout << "[B] thread id [async]: " << gettid() << endl;
        sleep(10);
        std::cout <<"[B] f1 exit!" <<endl;
        return 8;
    });
    cout << "[3] f1 create ok"<< endl;

    std::future<int> f2 = std::async(std::launch::deferred, []() {
        std::cout << "[C] thread id [deferred]: " << gettid() << endl;
        sleep(10);
        std::cout <<"[C] f0 exit!" <<endl;
        return 13;
    });
    cout << "[4] f2 create ok"<< endl;
    cout << "[5] end, will call f.get()..."<< endl;

    cout << f0.get() << endl; //output: 5
    cout << f1.get() << endl; //output: 8
    cout << f2.get() << endl; //output: 13
    cout << "[6] complete"<< endl;
}

void test_packaged_task(){


}


int main(int argc, char *argv[])
{
    test_async();
    getchar();
    return 0;
}

/**

[root@ec37128cebc0]# ./main
[0] main thread id: 281
[1] begin
[2] f0 create ok
[3] f1 create ok
[4] f2 create ok
[5] end, will call f.get()...
[A] thread id [async|deferred]: 282
[B] thread id [async]: 283
[A] f0 exit!
5
[B] f1 exit!
8
[C] thread id [deferred]: 281
[C] f0 exit!
13
[6] complete

 */
