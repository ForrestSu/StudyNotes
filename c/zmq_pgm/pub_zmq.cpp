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
    void *skt = zmq_socket(ctx, ZMQ_PUB);
    int ret = zmq_bind(skt, saddr.c_str());
    if (ret < 0) {
        printf("zmq_bind() rc = %d, <%s>\n", ret, strerror(errno));
        return 0;
    }

    int64_t iTotalSend = 0;
    std::string topic = "test";
    std::string msgdata = "msg data.";
    printf("pub %s, topic = <%s>\n", saddr.c_str(), topic.c_str());

    while (1) {
        int rc = zmq_send(skt, topic.c_str(), topic.size(), ZMQ_SNDMORE | ZMQ_DONTWAIT);
        if (rc < 0) {
            printf("[1] zmq_send error %d <%s>.\n", errno, strerror(errno));
            continue;
        }

        rc = zmq_send(skt, msgdata.c_str(), msgdata.size(), ZMQ_DONTWAIT);
        if (rc < 0) {
            int retry_cnt = 0;
            while (rc < 0 && retry_cnt < 3) {
                retry_cnt++;
                rc = zmq_send(skt, msgdata.c_str(), msgdata.size(), ZMQ_DONTWAIT);
            }
            if (rc < 0) {
                printf("[2] zmq_send() retry 3 times fail too! %d<%s>\n", errno, strerror(errno));
            }
        }
        ++iTotalSend;
        if (iTotalSend % printOncePerN == 0) {
            printf("[%ld] success pub %d nbytes. \n", iTotalSend, rc);
            usleep(1 * 1000); // 1 ms
        }
    }
    /* close skt*/
    zmq_close(skt);
    zmq_ctx_destroy(ctx);
    return 0;
}
