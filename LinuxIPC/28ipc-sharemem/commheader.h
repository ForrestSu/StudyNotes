#include <unistd.h> 
#include <sys/types.h>
#include <sys/msg.h>  

#include <sys/stat.h>
#include <fcntl.h>
//共享内存头文件
#include <sys/mman.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
    do \
    { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)


