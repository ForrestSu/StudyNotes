#include "commheader.h"

#define MSG_MAXLEN 8192
struct msgbuf{
    long mtype;
    char mtext[MSG_MAXLEN];
};

int echo_svr(int msgid)
{
    int n;
    struct msgbuf msg;
    memset(&msg,0,sizeof(struct msgbuf));
    while(1)
    {
        //只接收类型为1的消息,flag=0无消息时阻塞
        n=msgrcv(msgid,&msg,MSG_MAXLEN,1,0);
        if(n<0) 
            ERR_EXIT("client Close!");
        //msg.mtext前4个字节存储客户端进程号
        int pid = *((int *)msg.mtext);
        //fputs(msg.mtext+4,stdout);
        printf("new msg: byte=%d,type=%ld, mtext=%s",n-4,msg.mtype, msg.mtext+4);
        msg.mtype = pid;
        n= msgsnd(msgid,&msg,n,0);
        if(n<0)
            ERR_EXIT("sendMsg error!");
        memset(msg.mtext,0,MSG_MAXLEN);
    }
    return 0;
}

int main(int argc,char *argv[])
{  
    int msgid;
    msgid= msgget(1234,IPC_CREAT|0666);
    if(msgid==-1)
        ERR_EXIT("msg_get");
   
    echo_svr(msgid);
    return 0;
}
