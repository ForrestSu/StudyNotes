/**
 * wiki: https://en.wikipedia.org/wiki/Pragmatic_General_Multicast
 * reference: https://tools.ietf.org/html/rfc3208
 * local rfc3208:  docs/[PGM]-rfc3208.txt
 */

#include <errno.h>
#include <string.h>
#include <zmq.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <string>

struct __attribute__((packed)) Packet_t{
    int64_t id;
    char data[16];
};

void set_options(void *skt, int rate)
{
    //rate is Kbit/s
    int rc = zmq_setsockopt(skt, ZMQ_RATE, &rate, sizeof(rate));
    assert(rc == 0);

    int recv_intal = 60000; //60s
    rc = zmq_setsockopt(skt, ZMQ_RECOVERY_IVL, &recv_intal, sizeof(recv_intal));
    assert(rc == 0);

    // so_buffer
    int recv_buffer = 4 * 1024 * 1024; //4MB
    rc = zmq_setsockopt(skt, ZMQ_SNDBUF, &recv_buffer, sizeof(recv_buffer));
    assert(rc == 0);
    rc = zmq_setsockopt(skt, ZMQ_RCVBUF, &recv_buffer, sizeof(recv_buffer));
    assert(rc == 0);

    int ttl = 16;
    rc = zmq_setsockopt(skt, ZMQ_MULTICAST_HOPS, &ttl, sizeof(ttl));
    assert(rc == 0);

    // water line
    int hard_max_msg_size = 1000000000;
    rc = zmq_setsockopt(skt, ZMQ_RCVHWM, &hard_max_msg_size, sizeof(hard_max_msg_size));
    assert(rc == 0);
    rc = zmq_setsockopt(skt, ZMQ_SNDHWM, &hard_max_msg_size, sizeof(hard_max_msg_size));
    assert(rc == 0);
}

int main(int argc, char *argv[])
{
    if (argc < 5) {
        printf("Please input: %s <address> <rate> <printOncePerN> <MsgCount>\n", argv[0]);
        printf("rate is Kbit/s, 默认是千兆网卡 10^6 = 1Gbit/s.\n");
        printf("%s \"epgm://eth0;239.192.1.1:5555\" 1000000 1000 100001\n", argv[0]);
        return 0;
    }
    std::string saddr = argv[1];
    int rate = atoi(argv[2]);
    int printOncePerN = atoi(argv[3]);
    int TotalMsg = atoi(argv[4]);

    void *ctx = zmq_ctx_new();
    void *skt = zmq_socket(ctx, ZMQ_PUB);

    set_options(skt, rate);

    int ret = zmq_bind(skt, saddr.c_str());
    if (ret < 0) {
        printf("zmq_bind() rc = %d, <%s>\n", ret, strerror(errno));
        return 0;
    }

    int64_t iTotalSend = 0;
    const std::string topic = "test";
    const std::string data = "hello world!";
    const int msg_length = sizeof(Packet_t);
    Packet_t pub_data;
    memset(&pub_data, 0, msg_length);
    memcpy(pub_data.data, data.c_str(), data.size());

    printf("pub %s, topic = <%s>\n", saddr.c_str(), topic.c_str());

    while (iTotalSend < TotalMsg) {
        int rc = zmq_send(skt, topic.c_str(), topic.size(), ZMQ_SNDMORE | ZMQ_DONTWAIT);
        if (rc < 0) {
            printf("[1] zmq_send error %d <%s>.\n", errno, strerror(errno));
            continue;
        }

        pub_data.id = iTotalSend;
        rc = zmq_send(skt, &pub_data, msg_length, ZMQ_DONTWAIT);
        if (rc < 0) {
            int retry_cnt = 0;
            while (rc < 0 && retry_cnt < 3) {
                retry_cnt++;
                rc = zmq_send(skt, &pub_data, msg_length, ZMQ_DONTWAIT);
            }
            if (rc < 0) {
                printf("[2] zmq_send() retry 3 times fail too! %d<%s>\n", errno, strerror(errno));
            }
        }
        ++iTotalSend;
        if (iTotalSend % printOncePerN == 0) {
            printf("[%ld] success pub %d nbytes. \n", iTotalSend, rc);
            //usleep(100 * 1000); // 0.1s
        }
    }
    getchar();
    /* close skt*/
    zmq_close(skt);
    zmq_ctx_destroy(ctx);
    return 0;
}
