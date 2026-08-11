#include "commheader.h"

 union semun {
     int              val;    /* Value for SETVAL */
     struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
     unsigned short  *array;  /* Array for GETALL, SETALL */
     struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                  (Linux-specific) */
};

int sem_create(key_t key)
{
    int semid;
    // IPC_EXCL 只创建一次，如果已经存在，则报错
    semid= semget(key,1,IPC_CREAT|IPC_EXCL | 0666);
    if (semid ==-1)
        ERR_EXIT("semget--create");
    printf("create %d OK!\n",semid);
    return semid;
}
//打开一个信号量集
int sem_open(key_t key)
{
    int semid;
    semid= semget(key,0,0);
    if(semid==-1)
        ERR_EXIT("semget");
    return semid;
}

// 设置某个信号量的初始值
int sem_setval(int semid,int val)
{
    union semun su;
    su.val = val;
    int ret;
    ret = semctl(semid, 0, SETVAL,su);
    if (ret == -1)
        ERR_EXIT("sem_setval");
    return ret;
}

// 获取某个信号量的当前计数值
int sem_getval(int semid)
{
    int iret;
    iret = semctl(semid,0,GETVAL,0);
    if(iret==-1)
        ERR_EXIT("sem_getval");
    return iret;
}

//删除一个信号量
int sem_del(int semid)
{
    int ret;
    ret = semctl(semid, 0, IPC_RMID,0);
    return ret;
}
// P操作
int sem_p(int semid)
{
    struct sembuf sb={ 0, -1, 0};
    int ret;
    ret = semop(semid,&sb,1);
    if(ret == -1)
       ERR_EXIT("sem_op");
    return ret;
}
// V操作
int sem_v(int semid)
{
    struct sembuf sb={ 0, 1, 0};
    int ret;
    ret = semop(semid,&sb,1);
    if(ret == -1)
       ERR_EXIT("sem_op");
    return ret;
}

int main(int argc,char *argv[])
{  
    int semid;
    sem_create(1234);


    return 0;
}
