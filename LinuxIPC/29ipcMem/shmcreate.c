#include "commheader.h"

typedef struct stu{
    char name[32];
    int age;
}STU;

int main(int argc,char *argv[])
{  
    int shmid = shmget(1234, sizeof(STU),IPC_CREAT | 0666);
    if(shmid == -1)
        ERR_EXIT("shmget");
    printf("shmcreat=%d\n",shmid);
    STU * ptr;
    ptr = shmat(shmid, NULL, 0);
    if(ptr == (void *)-1 )
        ERR_EXIT("shmat");
    strcpy(ptr->name, "lisi");
    ptr->age = 20;

    sleep(10);
    //解除共享内存映射
    int iRet = shmdt(ptr);
    printf("deattach=%d\n", iRet);

    return 0;
}
