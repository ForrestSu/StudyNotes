#pragma once

#include <time.h>

struct timespec tp;

#define log_error(msg, ...)                                                 \
    do {                                                                    \
        clock_gettime(CLOCK_MONOTONIC, &tp);                                \
        if (errno > 0) {                                                    \
            fprintf(stderr, "[%ld.%ld] [ERROR](%s:%d) "msg" [%s:%d]\n",     \
                tp.tv_sec, tp.tv_nsec, __FILE__, __LINE__, ##__VA_ARGS__,   \
                strerror(errno), errno);                                    \
        }                                                                   \
        else {                                                              \
            fprintf(stderr, "[%ld.%ld] [ERROR](%s:%d)"msg"\n",              \
                tp.tv_sec, tp.tv_nsec, __FILE__, __LINE__, ##__VA_ARGS__);  \
        }                                                                   \
        fflush(stdout);                                                     \
    } while (0)


#define exit_error(...)                                                     \
    do { log_error(__VA_ARGS__); exit(EXIT_FAILURE); } while (0)


#define log_debug(msg, ...)                                                 \
    do {                                                                    \
        clock_gettime(CLOCK_MONOTONIC, &tp);                                \
        fprintf(stdout, "[%ld.%ld] [DEBUG] "msg"\n",                        \
            tp.tv_sec, tp.tv_nsec, ##__VA_ARGS__);                          \
        fflush(stdout);                                                     \
    } while (0)