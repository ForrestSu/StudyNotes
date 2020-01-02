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

void set_options(void *skt, int rate)
{
    int rc = 0;
    //rate is Kbit/s
    rc = zmq_setsockopt(skt, ZMQ_RATE, &rate, sizeof(rate));
    assert(rc == 0);

    int recv_intal = 60000;
    rc = zmq_setsockopt(skt, ZMQ_RECOVERY_IVL, &recv_intal, sizeof(recv_intal));
    assert(rc == 0);

    int recv_buffer = 4 * 1024 * 1024; //4MB
    rc = zmq_setsockopt(skt, ZMQ_SNDBUF, &recv_buffer, sizeof(recv_buffer));
    assert(rc == 0);
    rc = zmq_setsockopt(skt, ZMQ_RCVBUF, &recv_buffer, sizeof(recv_buffer));
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
    if (argc < 4) {
        printf("Please input: %s <address> <rate> <printOncePerN>\n", argv[0]);
        printf("rate is Kbit/s, 默认是千兆网卡 10^6 = 1Gbit/s.\n");
        printf("%s \"epgm://eth0;239.192.1.1:5555\" 1000000 1000\n", argv[0]);
        return 0;
    }
    std::string saddr = argv[1];
    int rate = atoi(argv[2]);
    int printOncePerN = atoi(argv[3]);

    void *ctx = zmq_ctx_new();
    void *skt = zmq_socket(ctx, ZMQ_SUB);

    set_options(skt, rate);

    std::string sub_topic = "test";
    int rc = zmq_setsockopt(skt, ZMQ_SUBSCRIBE, sub_topic.c_str(), sub_topic.size());
    assert(rc == 0);

    rc = zmq_connect(skt, saddr.c_str());
    if (rc < 0) {
        printf("zmq_connect() rc = %d, <%s>\n", rc, strerror(errno));
        return 0;
    }

    printf("sub %s, topic = <%s>\n", saddr.c_str(), sub_topic.c_str());

    int64_t iTotalRecv = 0;
    zmq_msg_t message;
    while (true) {
        zmq_msg_close(&message);
        zmq_msg_init(&message);
        rc = zmq_msg_recv(&message, skt, 0);
        if (rc < 0) {
            printf("recv fail! rc == %d, errno = %d\n", rc, errno);
            usleep(100 * 1000); //wait 0.1s
            continue;
        }

        std::string topic((char*) zmq_msg_data(&message), zmq_msg_size(&message));
        int is_more = zmq_msg_more(&message);

        assert(is_more == 1);

        iTotalRecv++;
        int this_frame_idx = 1;
        while (is_more) {
            zmq_msg_close(&message);
            zmq_msg_init(&message);
            rc = zmq_msg_recv(&message, skt, 0);
            if (rc < 0) {
                printf("recv2 fail ! rc == %d, errno = %d\n", rc, errno);
                break;
            }

            is_more = zmq_msg_more(&message);
            std::string msg_data((char*) zmq_msg_data(&message), zmq_msg_size(&message));

            if ((iTotalRecv % printOncePerN == 1) || this_frame_idx > 1) {
                printf("[%ld], topic <%s>, data <%s>\n", iTotalRecv, topic.c_str(), msg_data.c_str());
            }
            //TODO judge have 3 frame?
            if (is_more) {
                this_frame_idx++;
                printf("[%ld], topic <%s>, rc=%d, data <%s>\n", iTotalRecv, topic.c_str(), rc, msg_data.c_str());
                printf("exception: bigger than 2 frame! this_frame_idx = %d.\n", this_frame_idx);
            } else {
                this_frame_idx = 0;
            }
        }
        //simulate recv slowly
        //sleep(3);
    }
    zmq_close(skt);
    zmq_ctx_destroy(ctx);
}
