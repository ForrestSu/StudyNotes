#include "commheader.h"

#define MSG_MAXLEN 65536
struct msgbuf{
    long mtype;
    char mtext[1];
};

int main(int argc,char *argv[])
{  
    int flag=0;
    int type =0;
    int opt=0;
    while(opt!=-1)
    {
        //n respent -n; t respent -t; : respent a 参数
        //opt == -1 表示参数解析完毕 
        opt = getopt(argc, argv,"nt:");
        if(opt=='?')
            exit(EXIT_FAILURE);
        switch(opt)
        {
            case 'n':
                flag |= IPC_NOWAIT;
                break;
            case 't':
                type = atoi(optarg);
                printf("type==%d\n",type);
                break;
        }
    }

    int msgid;
    msgid= msgget(1234,0);
    if(msgid==-1)
        ERR_EXIT("msg_get");
    struct msgbuf *ptr;
    ptr= (struct msgbuf*)malloc(sizeof(long) + MSG_MAXLEN);
    ptr->mtype=type;
    int iret=msgrcv(msgid,ptr,MSG_MAXLEN, type,flag);
    if(iret<0)
      ERR_EXIT("recv Msg error:");
    printf("msg type=%ld , len=%d\n",ptr->mtype,iret);
    return 0;


}
