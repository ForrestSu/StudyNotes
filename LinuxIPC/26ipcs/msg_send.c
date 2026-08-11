#include "commheader.h"

struct msgbuf{
    long mtype;
    char mtext[1];
};

int main(int argc,char *argv[])
{
    if (argc!=3)
    {
        fprintf(stderr,"Usag:%s <type> <bytes>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    long type = atoi(argv[1]);
    int len= atoi(argv[2]);
    int msgid;
    msgid= msgget(1234,0);
    if(msgid==-1)
        ERR_EXIT("msg_get");
    struct msgbuf *ptr;
    ptr= (struct msgbuf*)malloc(sizeof(long)+len);
    ptr->mtype=type;
    //int iret=msgsnd(msgid,ptr,len, IPC_NOWAIT);
    int iret=msgsnd(msgid,ptr,len, 0);
    if(iret<0)
      ERR_EXIT("send msg error:");
    return 0;
}
