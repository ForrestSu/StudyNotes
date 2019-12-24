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

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Please input: %s <address> <printOncePerN>\n", argv[0]);
        printf("%s \"epgm://eth0;239.192.1.1:5555\" 1000\n", argv[0]);
        return 0;
    }
    std::string saddr = argv[1];
    int printOncePerN = atoi(argv[2]);

    void *ctx = zmq_ctx_new();
    void *skt = zmq_socket(ctx, ZMQ_SUB);
    int rc = zmq_connect(skt, saddr.c_str());
    if (rc < 0) {
        printf("zmq_connect() rc = %d, <%s>\n", rc, strerror(errno));
        return 0;
    }

    std::string sub_topic = "test";
    rc = zmq_setsockopt(skt, ZMQ_SUBSCRIBE, sub_topic.c_str(), sub_topic.size());
    assert(rc == 0);

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
                printf("exception: bigger than 2 frame! this_frame_idx = %d.\n", this_frame_idx);
            } else {
                this_frame_idx = 0;
            }
        }
        iTotalRecv++;
        //simulate recv slowly
        //sleep(3);
    }
    zmq_close(skt);
    zmq_ctx_destroy(ctx);
}
