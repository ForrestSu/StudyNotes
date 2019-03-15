/*
 * test_zmq_inproc_perf.cpp
 * 测试zmq inproc 发送接收效率, (取代传统的进程间通信带锁的方式)
 * 
 * Created on: 2019年03月14日
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <atomic>
#include <vector>
#include <string>
#include <zmq.h>
#include <chrono>

#include <pthread.h>
#include <unistd.h>
#include <cassert>

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
int64_t global_start_time_ms = 0;
// 创建 并设置socket 的属性 (发送和接收水位)
void * CreateSocket(void* context, int type);

/**
 * 生产者线程
 *  发送大量 8 个字节的数据
 */
static void *
producer_routine(void *m_socket)
{
    printf("start send thread!\n");
    int k = 0;
    int success_cnt = 0;
    const int64_t msg_data =  0x12345678;
    const int msg_len = sizeof(msg_data);
    if(global_start_time_ms <1){
        global_start_time_ms = CurrentTimeMillis();
    }
    while (k < test_count) {
        zmq_msg_t msg;
        int rc = zmq_msg_init_size(&msg, msg_len);
        assert(rc == 0);
        memcpy(zmq_msg_data(&msg), &msg_data, msg_len);
        int nbytes = zmq_msg_send(&msg, m_socket, ZMQ_DONTWAIT);
        if (nbytes == 8) {
            ++success_cnt;
        } else {
            fprintf(stderr, "send error %d, errno = %d!\n", nbytes, errno);
        }
        ++k;
        zmq_msg_close(&msg);
    }
    auto end_time_ms = CurrentTimeMillis();
    printf("zmq_inproc_send : cost time %lld ms.\n", (end_time_ms - global_start_time_ms));
    printf("zmq_inproc_send : k = %d, success_cnt = %d.\n", k, success_cnt);
    return NULL;
}

void start_consumer(void *m_socket, int recv_count_total)
{
    int success_cnt = 0;
    int64_t fail_count = 0;
    int64_t fail_count_eagain = 0;
    // 开始接收数据
    while (true) {
        zmq_msg_t msg;
        zmq_msg_init(&msg);
        int nbytes = zmq_msg_recv(&msg, m_socket, ZMQ_DONTWAIT);
        if (nbytes < 0) {
            ++fail_count;
            //printf("error : 1 recv rc = %d \n", nbytes);
            continue;
        }
        int more = zmq_msg_more(&msg);
        if(more){
           zmq_msg_close(&msg);
           zmq_msg_init(&msg);
           nbytes = zmq_msg_recv(&msg, m_socket, ZMQ_DONTWAIT);
          // printf("%d is more\n", more);
        }
        if (nbytes == 8) {
            ++success_cnt;
            // 接收完毕
            if (success_cnt >= recv_count_total) {
                int64_t* pdata = (int64_t*)(zmq_msg_data(&msg));
                printf("last recv 0x%x, nbytes %d!\n ", *pdata, nbytes);
                break;
            }
            zmq_msg_close(&msg);
        } else {
            printf("error : 2 recv rc = %d \n", nbytes);
            if (errno == EAGAIN) {
                ++fail_count_eagain;
            }
            if(nbytes > 0){
                int32_t* pdata = (int32_t*)(zmq_msg_data(&msg));
                printf("recv nbytes = %d, msg_size %d, msg_data = %x\n", nbytes,  zmq_msg_size(&msg), *pdata);

            }
        }
    }
    auto end_time_ms = CurrentTimeMillis();
    printf("zmq_inproc_recv : cost time %lld ms.\n", (end_time_ms - global_start_time_ms));
    printf("zmq_inproc_recv : test_count = %d, success_cnt = %d.\n", recv_count_total, success_cnt);
    printf("zmq_inproc_recv : fail_count %lld , fail_count_eagain = %lld.\n",  fail_count, fail_count_eagain);
}
/**
 * TODO 测试发现,先bind 或者先 connect 的先后顺序 不影响程序运行
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("sizeof(int64_t) %d\n", sizeof(int64_t));
        puts("error: input test_count!");
        return 0;
    }
    test_count = atoi(argv[1]);

    void *context = zmq_ctx_new();
    void *m_recv_sock = CreateSocket(context, ZMQ_PULL);
    zmq_bind(m_recv_sock, "inproc://workers");

    void *m_send_sock = CreateSocket(context, ZMQ_PUSH);
    int rc = zmq_connect(m_send_sock, "inproc://workers");
    printf("connect rc = %d\n", rc);
    pthread_t tid;
    pthread_create(&tid, NULL, producer_routine, m_send_sock);

    void *m_send_sock2 = CreateSocket(context, ZMQ_PUSH);
    rc = zmq_connect(m_send_sock2, "inproc://workers");
    pthread_t tid2;
    pthread_create(&tid2, NULL, producer_routine, m_send_sock2);
    //启动生产者
    start_consumer(m_recv_sock, test_count*2);

    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    //  We never get here, but clean up anyhow
    zmq_close(m_send_sock);
    zmq_close(m_recv_sock);
    zmq_ctx_destroy(context);
    return 0;
}

/********/
void * CreateSocket(void* context, int type)
{
    void *m_socket = zmq_socket(context, type);
    int iTimeOut = 0;
    size_t sizeTimeOut = sizeof(iTimeOut);
    zmq_setsockopt(m_socket, ZMQ_SNDTIMEO, &iTimeOut, sizeTimeOut);
    zmq_setsockopt(m_socket, ZMQ_RCVTIMEO, &iTimeOut, sizeTimeOut);

    // water line
    int iHWM = 100000000;
    size_t sizeHWM = sizeof(iHWM);
    zmq_setsockopt(m_socket, ZMQ_SNDHWM, &iHWM, sizeHWM);
    zmq_setsockopt(m_socket, ZMQ_RCVHWM, &iHWM, sizeHWM);

    // reconnect interval
    int iReconnectInterval = 10;
    size_t sizeReconnect = sizeof(iReconnectInterval);
    zmq_setsockopt(m_socket, ZMQ_RECONNECT_IVL, &iReconnectInterval, sizeReconnect);

    // max reconnect interval
    int iMaxReconnectInterval = 2000;
    size_t sizeReconnectMax = sizeof(iMaxReconnectInterval);
    zmq_setsockopt(m_socket, ZMQ_RECONNECT_IVL_MAX, &iMaxReconnectInterval, sizeReconnectMax);

    // tcp heartbeat
    int iTcpHeartBeat = 1;
    size_t sizeTcpHeartBeat = sizeof(iTcpHeartBeat);
    zmq_setsockopt(m_socket, ZMQ_TCP_KEEPALIVE, &iTcpHeartBeat, sizeTcpHeartBeat);

    int iTcpIdle = 1;
    size_t sizeTcpIdle = sizeof(iTcpIdle);
    zmq_setsockopt(m_socket, ZMQ_TCP_KEEPALIVE_IDLE, &iTcpIdle, sizeTcpIdle);

    int iTcpCnt = 3;
    size_t sizeTcpCnt = sizeof(iTcpCnt);
    zmq_setsockopt(m_socket, ZMQ_TCP_KEEPALIVE_CNT, &iTcpCnt, sizeTcpCnt);

    int iTcpIntvl = 3;
    size_t sizeTcpIntvl = sizeof(iTcpIntvl);
    zmq_setsockopt(m_socket, ZMQ_TCP_KEEPALIVE_INTVL, &iTcpIntvl, sizeTcpIntvl);

    int iZmqLiner = 0;
    size_t sizeLiner = sizeof(iZmqLiner);
    zmq_setsockopt(m_socket, ZMQ_LINGER, &iZmqLiner, sizeLiner);
    return m_socket;
}

/**
 * SOCKET 类型使用 push/pull 效率 和 ROUTER/DEALER 差不多,只是 ROUTER/DEALER 每次会发送2 帧
connect rc = 0
start send thread!
last recv 0x12345678, nbytes 8!
 zmq_inproc_recv : cost time 2738 ms.
zmq_inproc_recv : test_count = 10000000, success_cnt = 10000000.
zmq_inproc_recv : fail_count 339851 , fail_count_eagain = 0.
zmq_inproc_send : cost time 2738 ms.
zmq_inproc_send : k = 10000000, success_cnt = 10000000.
 */
