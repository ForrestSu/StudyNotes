#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/**
* gcc clock_gettime_test.cpp -Wall -O2 -o clock_gettime
*/
int main()
{
    struct timespec cur_time = { 0, 0 };
    // 1 系统实时时间, 随系统实时时间改变而改变(since 1970-01-01)
    clock_gettime(CLOCK_REALTIME, &cur_time);
    printf("CLOCK_REALTIME: %lu, %lu\n", cur_time.tv_sec, cur_time.tv_nsec);
    // 2 从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
    clock_gettime(CLOCK_MONOTONIC, &cur_time);
    printf("CLOCK_MONOTONIC: %lu, %lu\n", cur_time.tv_sec, cur_time.tv_nsec);

    // 3 本进程到当前代码系统 CPU 时间
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur_time);
    printf("CLOCK_PROCESS_CPUTIME_ID: %lu, %lu\n", cur_time.tv_sec, cur_time.tv_nsec);

    // 4 本进程到当前代码系统 CPU 时间
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cur_time);
    printf("CLOCK_THREAD_CPUTIME_ID: %lu, %lu\n", cur_time.tv_sec, cur_time.tv_nsec);
    printf("\ntime(NULL) == %lu sec\n", time(NULL));
    
    cur_time.tv_sec = 8888888;
    int rc  = clock_settime(CLOCK_MONOTONIC, (const struct timespec*)&cur_time);
    printf("rc %d, set CLOCK_MONOTONIC: %lu, %lu\n", rc, cur_time.tv_sec, cur_time.tv_nsec);
    if(rc <0){
        printf("error = %d, <%s>\n", errno, strerror(errno));
    }
    
    rc = clock_gettime(CLOCK_MONOTONIC, &cur_time);
    printf("rc %d, get CLOCK_MONOTONIC: %lu, %lu\n", rc, cur_time.tv_sec, cur_time.tv_nsec);
    //getchar();
    return 0;
}