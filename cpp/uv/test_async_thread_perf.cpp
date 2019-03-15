/*
 *  g++ -std=c++11 -Wall -O2 test_async_thread_perf.cpp  -lpthread -luv  -o async_test
 * test_async_thread_perf.cpp
 * 测试Async 加锁 发送的效率（进程间通信的方式）
 * 
 * Created on: 2019年03月14日
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uv.h>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>


class spinlock {
private:
    typedef enum {
        Locked, Unlocked
    } LockState;
    std::atomic<LockState> state_;

public:
    spinlock() :
            state_(Unlocked)
    {
    }
    void lock()
    {
        while (state_.exchange(Locked, std::memory_order_acquire) == Locked) {
            /* busy-wait */
        }
    }
    void unlock()
    {
        state_.store(Unlocked, std::memory_order_release);
    }
};

int64_t CurrentTimeMillis()
{
//#ifdef _WIN32
    int64_t timems = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return timems;
}

using namespace std;

int test_count = 1000000;
uv_async_t async_hdl;
vector<int64_t> global_vec;
spinlock m_lock;

void close_cb(uv_handle_t* handle);
void on_async_cb(uv_async_t* handle);
void producer_thread(void* arg);
 

void close_cb(uv_handle_t* handle)
{
    printf("close the async handle!\n");
}
 

int64_t global_start_time_ms = 0;
int64_t total_consumer = 0;

// 在主线程中运行
void on_async_cb(uv_async_t* handle)
{
    vector<int64_t> tmp_data;
    m_lock.lock();
    global_vec.swap(tmp_data);
    m_lock.unlock();

    total_consumer += tmp_data.size();
    if (total_consumer >= test_count) {
        auto end_time_ms = CurrentTimeMillis();
        printf("on_async_cb total cost %lld ms (from send).\n",  (end_time_ms - global_start_time_ms));
        printf("on_async_cb : total_consumer = %lld.\n", total_consumer);
    }
}

// 循环发送 Async 消息
int DoAsync(int cur_id)
{
    // 加锁放入数据
    m_lock.lock();
    global_vec.push_back(cur_id);
    m_lock.unlock();

    async_hdl.data = (void *)100;
    int rc = uv_async_send(&async_hdl);
    return rc;
}

/**
 * 生产者线程
 */
void producer_thread(void* arg)
{
    int k = 0, success_cnt = 0;
    uv_thread_t id = uv_thread_self();
    printf("sub thread id:%lu.\n", id);
    if (global_start_time_ms < 1) {
        global_start_time_ms = CurrentTimeMillis();
    }
    while(k < test_count)
    {
        int rc = DoAsync(k);
        if(rc == 0)
            ++success_cnt;
        ++k;
    }
    auto end_time_ms = CurrentTimeMillis();
    printf("send async : cost time %lld ms.\n", (end_time_ms - global_start_time_ms));
    printf("send async : k = %d, success_cnt = %d.\n", k, success_cnt);
}

void timer_cb(uv_timer_t *handle) {
     printf("timer total_consumer = %lld.\n", total_consumer);
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        puts("error: input test_count!");
        return 0;
    }
    test_count = atoi(argv[1]);

    uv_loop_t* uvloop = uv_default_loop();
 
    uv_thread_t id = uv_thread_self();
    printf("thread id:%lu.\n", id);
 
    //初始化 async_handle, 并注册回调函数
    uv_async_init(uvloop, &async_hdl, on_async_cb);
 
    uv_timer_t timer_hdl;
    uv_timer_init(uvloop, &timer_hdl);
    uv_timer_start(&timer_hdl, timer_cb, 0, 2000);
    //创建子线程,发送Async通知
    uv_thread_t thread;
    uv_thread_create(&thread, producer_thread, NULL);

    // uv_thread_t thread2;
    //uv_thread_create(&thread2, producer_thread, NULL);
 
    uv_run(uvloop, UV_RUN_DEFAULT);
    // 等待子线程完成
    uv_thread_join(&thread);
    //uv_thread_join(&thread2);
    return 0;
}


/* 启动一个线程
*
thread id:140110925088576.
timer total_consumer = 0.
sub thread id:140110899500800.
send async : cost time 802 ms.
send async : k = 10000000, success_cnt = 10000000.
on_async_cb total cost 802 ms (from send).
on_async_cb : total_consumer = 10000000.
timer total_consumer = 10000000.
*/


/* 启动两个线程之后
thread id:140555206666048.
sub thread id:140555181078272.
timer total_consumer = 0.
sub thread id:140555172685568.
send async : cost time 1675 ms.
send async : k = 5000000, success_cnt = 5000000.
send async : cost time 1754 ms.
send async : k = 5000000, success_cnt = 5000000.
on_async_cb total cost 1776 ms (from send).
on_async_cb : total_consumer = 10000000.
timer total_consumer = 10000000.
timer total_consumer = 10000000.
*/
