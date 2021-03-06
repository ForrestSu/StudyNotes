#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>

#include "helper.h"

#define NUM_PRODUCERS 4
#define NUM_CONSUMERS 2
#define MAX_EVENTS_SIZE 1024


typedef struct thread_info {
    pthread_t thread_id;
    int rank;
    int epfd;
} thread_info_t;


static void do_task() {
    return;
}


static void *consumer_routine(void *data) {
    struct thread_info *c = (struct thread_info *)data;
    struct epoll_event *events;
    int epfd = c->epfd;
    int nfds = -1;
    int i = -1;
    int ret = -1;
    uint64_t v;
    int num_done = 0;

    events = calloc(MAX_EVENTS_SIZE, sizeof(struct epoll_event));
    if (events == NULL) exit_error("calloc epoll events\n");

    for (;;) {
        nfds = epoll_wait(epfd, events, MAX_EVENTS_SIZE, 1000);
        for (i = 0; i < nfds; i++) {
            if (events[i].events & EPOLLIN) {
                log_debug("[consumer-%d] got event from fd-%d",
                        c->rank, events[i].data.fd);
                ret = read(events[i].data.fd, &v, sizeof(v));
                if (ret < 0) {
                    log_error("[consumer-%d] failed to read timerfd", c->rank);
                    continue;
                }
                do_task();
                log_debug("[consumer-%d] tasks done: %d", c->rank, ++num_done);
            }
        }
    }
}


static void *producer_routine(void *data) {
    struct thread_info *p = (struct thread_info *)data;
    struct epoll_event event;
    int epfd = p->epfd;
    int tfd = -1;
    int ret = -1;

    struct itimerspec its;
    its.it_value.tv_sec = 1;    // initial expiration
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 3; // interval
    its.it_interval.tv_nsec = 0;

    log_debug("[producer-%d] Greetings!", p->rank);
    tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
    if (tfd == -1) exit_error("timerfd create: %s", strerror(errno));
    event.data.fd = tfd;
    event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, tfd, &event);
    if (ret != 0) exit_error("epoll_ctl");
    ret = timerfd_settime(tfd, 0, &its, NULL);
    if (ret != 0) exit_error("timerfd settime");

    return (void *)0;
}


int main(int argc, char *argv[]) {
    struct thread_info *p_list = NULL, *c_list = NULL;
    int epfd = -1;
    int ret = -1, i = -1;

    // create epoll fd
    epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) exit_error("epoll_create1: %s", strerror(errno));

    // start consumers (as task worker)
    c_list = calloc(NUM_CONSUMERS, sizeof(struct thread_info));
    if (!c_list) exit_error("calloc");
    for (i = 0; i < NUM_CONSUMERS; i++) {
        c_list[i].rank = i;
        c_list[i].epfd = epfd;
        ret = pthread_create(&c_list[i].thread_id, NULL, consumer_routine, &c_list[i]);
        if (ret != 0) exit_error("pthread_create");
    }

    // start producers (as test load)
    p_list = calloc(NUM_PRODUCERS, sizeof(struct thread_info));
    if (!p_list) exit_error("calloc");
    for (i = 0; i < NUM_PRODUCERS; i++) {
        p_list[i].rank = i;
        p_list[i].epfd = epfd;
        ret = pthread_create(&p_list[i].thread_id, NULL, producer_routine, &p_list[i]);
        if (ret != 0) exit_error("pthread_create");
    }

    // join and exit
    for (i = 0; i < NUM_PRODUCERS; i++) {
        ret = pthread_join(p_list[i].thread_id, NULL);
        if (ret != 0) exit_error("pthread_join");
    }
    for (i = 0; i < NUM_CONSUMERS; i++) {
        ret = pthread_join(c_list[i].thread_id, NULL);
        if (ret != 0) exit_error("pthread_join");
    }

    free(p_list);
    free(c_list);

    return EXIT_SUCCESS;
}
