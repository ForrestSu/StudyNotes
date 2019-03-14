/*
 * test_async_thread_perf.cpp
 * 测试zmq inproc 发送接收效率, (取代传统的进程间通信带锁的方式)
 * 
 * Created on: 2019年03月14日
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uv.h>
#include <atomic>
#include <vector>
#include <string>
#include <zmq.h>

#include <pthread.h>
#include <unistd.h>


int64_t CurrentTimeMillis()
{
//#ifdef _WIN32
    int64_t timems = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return timems;
}

using namespace std;
/**
 * g++ -std=c++11 -Wall -O2 test_zmq_inproc_perf.cpp  -lpthread -lzmq  -o inproc_test
 */

int test_count = 1000000;


typedef TPubData_t{
 int32_t type;
 void* data;
}__attribute__ ((packed)) pub_data_t;

/**
 * 生产者线程
 *  发送大量12个字节的数据
 */
static void *
producer_routine (void *msend) 
{
    int rc = zmq_connect(msend, "inproc://workers");
    printf("inproc_send : rc = %d \n", rc);

    int k = 0; 
    int success_cnt = 0;
    auto start_time_ms = CurrentTimeMillis();
    while (k < test_count) {
        pub_data_t pMsg;
        pMsg.data = &pMsg;
        int rc = zmq_send(msend, (void*)(&pMsg) , sizeof(pub_data_t), ZMQ_DONTWAIT);
        if(rc >= 0 ){
            ++success_cnt;
        }
        ++test_count;
    }
    auto end_time_ms = CurrentTimeMillis();
    printf("zmq_inproc_send : cost time %lld ms.\n", (end_time_ms - start_time_ms));
    printf("zmq_inproc_send : k = %d, success_cnt = %d.\n", k, success_cnt);
    zmq_close (msend);
    return NULL;
}

void start_consumer(void *workers)
{    
    int success_cnt = 0;
    auto start_time_ms = CurrentTimeMillis();
    // 开始接收数据
    while(true){
        char buf[16] = {0};
        nbytes = zmq_recv (workers, buf, 16, ZMQ_DONTWAIT);
        if(nbytes >= 12){
            ++success_cnt;
            // 接收完毕
            if(success_cnt >= test_count){
                break;
            }
        }else if(nbytes < 0){
            // EAGAIN
        }
    }
    auto end_time_ms = CurrentTimeMillis();
    printf("zmq_inproc_recv : cost time %lld ms.\n", (end_time_ms - start_time_ms));
    printf("zmq_inproc_recv : test_count = %d, success_cnt = %d.\n", test_count, success_cnt);
} 

//FIXME 设置socket 的属性 (发送和接收水位)
//
//FIXME 还得测试先启动client 的情况
void * CreateSocket(void* context, int type)
{
     void *m_sock = zmq_socket(context, type);

     
     return m_sock;
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        puts("error: input test_count!");
        return 0;
    }
    test_count = atoi(argv[1]);

    void *context = zmq_ctx_new ();
    void *m_recv_sock = CreateSocket(context, ZMQ_ROUTER);
    zmq_bind(workers, "inproc://workers");


    void *m_send_sock = CreateSocket(context, ZMQ_DEALER);
    //  Launch pool of worker threads
    pthread_t tid;
    pthread_create (&tid, NULL, producer_routine, m_send_sock);

    //启动生产者
    start_consumer(m_recv_sock);

    pthread_join(tid, NULL);
    //  We never get here, but clean up anyhow
    zmq_close (clients);
    zmq_close (workers);
    zmq_ctx_destroy (context);
    return 0;
}