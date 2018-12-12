/*
 * test_async_thread.cpp
 *
 * Created on: 2015年2月10日
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uv.h>
#include <atomic>
 

/**
 * g++ -std=c++11 -Wall -g test_async_thread.cpp  -lpthread -luv  -o async_test
 */
const int test_count = 1000'000;
std::atomic_int total_active{0};
uv_async_t async_hdl;

void close_cb(uv_handle_t* handle);
void async_cb(uv_async_t* handle);
void another_thread(void* arg);
 
void close_cb(uv_handle_t* handle)
{
    printf("close the async handle!\n");
}
 

// 回调函数
void async_cb(uv_async_t* handle)
{
    void* myptr =  handle->data;
    uv_thread_t tid = uv_thread_self();
    ++total_active;
    //printf("async_cb called, thread_id = %lu, myptr = %d\n", tid, myptr);
    //uv_close((uv_handle_t*)&async, close_cb);    //如果async没有关闭，消息队列是会阻塞的
}

/**
 *
 */
void another_thread(void* arg)
{
    int icount = 0, success_cnt = 0;
    uv_thread_t id = uv_thread_self();
    printf("sub thread id:%lu.\n", id);
    int rc = 0;
    while(icount < test_count)
    {
        async_hdl.data = (void *)100;
        rc = uv_async_send(&async_hdl);
        if(rc == 0)
            ++success_cnt;
        ++icount;
    }
    printf("icount = %d ,success_cnt = %d\n", icount, success_cnt);
}

void timer_cb(uv_timer_t *handle) {
     printf("timer total_active = %d\n", total_active.load());
}

/**
*  主要测试 uv_async 多次调用, 实际上 on_async 很多时候不会真正触发
*  所以我们在调用 uv_async 的时候,可能需要用 很小的lock 将data放入vector
*  在 on_async 将vector swap 出来(即使on_async 真正调用的次数 少于uv_async 也不会丢失数据)
**/

int main()
{
    uv_loop_t* uvloop = uv_default_loop();
 
    uv_thread_t id = uv_thread_self();
    printf("thread id:%lu.\n", id);
 
    //初始化 async_handle, 并注册回调函数
    uv_async_init(uvloop, &async_hdl, async_cb);
 
    uv_timer_t timer_hdl;
    uv_timer_init(uvloop, &timer_hdl);
    uv_timer_start(&timer_hdl, timer_cb, 0, 2000);
    
    //创建子线程,发送Async通知
    uv_thread_t thread;
    uv_thread_create(&thread, another_thread, NULL);
 
    uv_run(uvloop, UV_RUN_DEFAULT);
    // 等待子线程完成
    uv_thread_join(&thread);
    return 0;
}
