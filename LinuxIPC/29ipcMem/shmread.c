#include "commheader.h"

typedef struct stu{
    char name[32];
    int age;
}STU;

int main(int argc,char *argv[])
{  
    int shmid = shmget(1234, 0, 0);
    if(shmid == -1)
        ERR_EXIT("shmget");
    printf("shmget=%d\n",shmid);
    STU * ptr;
    ptr = shmat(shmid, NULL, 0);
    if(ptr == (void *)-1 )
        ERR_EXIT("shmat");
    printf("name=%s ,age=%d\n", ptr->name,ptr->age);
    
    sleep(10);
    //解除共享内存映射
    int iRet = shmdt(ptr);
    printf("deattach=%d\n", iRet);
    // 删除共享内存，最后一个参数可以传NULL 
    shmctl(shmid,IPC_RMID,NULL);

    return 0;
}
