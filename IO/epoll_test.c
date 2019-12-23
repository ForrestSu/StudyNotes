/**
 * terminal1
 * >./app 127.0.0.1 8000 
 * 
 * terminal2 
 * nc 127.0.0.1 8000
 * > helloworld
 * >
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h> // inet_pton
#include <sys/socket.h>
#include <sys/epoll.h>

#define IS_NON_BLOCK 1

/* 出错处理 */
void err_exit(char *msg)
{
    perror(msg);
    exit(1);
}
/**
 * AF  Address Family
 * PF  Protocol Family
 * AF和 PF没有区别,  对于BSD 是AF, 对于POSIX是PF.
 */
int create_socket(const char *ip, const int iport)
{
    struct sockaddr_in server_addr;
    int fd;
    int rc = 0;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(iport);

    rc = inet_pton(AF_INET, ip, &server_addr.sin_addr);
    if (rc == -1) {
        err_exit("inet_pton() error");
    }

    fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        err_exit("socket() error");
    }

    /** //socket 设置端口重用
     int reuse = 1;
     rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
     if (rc == -1) {
     err_exit("setsockopt() error");
     }*/

    rc = bind(fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (rc == -1) {
        err_exit("bind() error");
    }

    rc = listen(fd, 5);
    if (rc == -1) {
        err_exit("listen() error");
    }
    return fd;
}

/* 设置文件为非阻塞 */
int set_nonblock(int fd)
{
    int old_flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, old_flags | O_NONBLOCK);
    return old_flags;
}

/* 注册文件描述符到 epoll，并设置其事件为EPOLLIN(可读事件) */
int addfd_to_epoll(int epoll_fd, int fd, int block_type)
{
    struct epoll_event ep_event;
    ep_event.data.fd = fd;
    ep_event.events = EPOLLIN;

    /* 缺省为LevelTrigger, 设置为 edge trigger*/
    ep_event.events |= EPOLLET;

    /* 设置是否阻塞 */
    if (block_type == IS_NON_BLOCK) {
        set_nonblock(fd);
    }
    int rc = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ep_event);
    return rc;
}

void epoll_et_loop(int fd)
{
    const int MAX_BUFFER_SIZE = 5;
    char buffer[MAX_BUFFER_SIZE + 1];
    memset(buffer, 0, sizeof(buffer));
    int nbytes;
    int total_bytes = 0;

    printf("[START] recv from fd = %d...\n", fd);
    while (1) {
        nbytes = recv(fd, buffer, MAX_BUFFER_SIZE, 0);
        if (nbytes > 0) {
            buffer[nbytes] = '\0';
            total_bytes += nbytes;
            printf("[et_loop] recv %d bytes <%s>.\n", nbytes, buffer);
        } else if (nbytes == 0) {
            // 对端关闭连接
            printf("Closed by client! fd = %d\n", fd);
            close(fd);
            break;
        } else {
            assert(nbytes == -1);
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("This batch total recv %d! errno = [%d]!\n", total_bytes, errno);
            } else {
                printf("recv on socket fd =%d error! errno = %d<%s>.\n", fd, errno, strerror(errno));
                close(fd);
            }
            break;
        }
    }
    printf("[END] recv from fd = %d...\n", fd);
}

void process_recv(int epollfd, struct epoll_event *events, int n, int listen_fd)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int conn_fd;
    int i;
    for (i = 0; i < n; i++) {

        // 如果是新的连接,需要把新的socket添加到 epollfd 中
        if (events[i].data.fd == listen_fd) {
            conn_fd = accept(listen_fd, (struct sockaddr*) &client_addr, &addr_len);
            printf("Accept() new conn_fd = %d...\n", conn_fd);

            addfd_to_epoll(epollfd, conn_fd, IS_NON_BLOCK);
            printf("Add into epoll_fd.\n");
        }
        // 否则,读取数据
        else {
            if (events[i].events & EPOLLIN) {
                conn_fd = events[i].data.fd;

                /* 带循环的ET模式 */
                epoll_et_loop(conn_fd);
            } else {
                printf("RECV UN-Support event = %d...\n", events[i].events);
            }
        }
    }
}

int main(int argc, const char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "usage: %s 127.0.0.1 8000\n", argv[0]);
        exit(1);
    }
    int port = atoi(argv[2]);
    int listen_fd;
    int epoll_fd;

    listen_fd = create_socket(argv[1], port);
    epoll_fd = epoll_create1(0);

    printf("listen_fd = %d, epoll_fd = %d.\n", listen_fd, epoll_fd);
    //将listen_socket_fd 添加到 epollfd 中
    addfd_to_epoll(epoll_fd, listen_fd, IS_NON_BLOCK);

    const int MAX_EVENT_FD_SIZE = 10;
    struct epoll_event events[MAX_EVENT_FD_SIZE];
    for (;;) {
        // wait
        int nready = epoll_wait(epoll_fd, events, MAX_EVENT_FD_SIZE, -1);
        if (nready == 0) {
            printf("time out..\n");
        } else if (nready > 0) {

            // process new connecting, or recv data
            process_recv(epoll_fd, events, nready, listen_fd);

        } else {
            assert(nready == -1);
            printf("fatal error %d <%s>.\n", errno, strerror(errno));
            break;
        }
    }
    return 0;
}
