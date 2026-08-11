#include "commheader.h"

#define MSG_MAXLEN 8192
struct msgbuf{
    long mtype;
    char mtext[MSG_MAXLEN];
};

int echo_cli(int msgid)
{
    int n;
    struct msgbuf msg;
    memset(&msg,0,sizeof(struct msgbuf));
    int pid=getpid();
    *((int*)msg.mtext)=pid;//前4个字节为pid
    while(fgets(msg.mtext+4,MSG_MAXLEN-4,stdin)!=NULL)
    {
        msg.mtype=1;
        n= msgsnd(msgid,&msg,4 + strlen(msg.mtext+4),0);
        if(n<0)
             ERR_EXIT("Client sendMsg error!");
        //接收类型为pid的消息
        memset(msg.mtext+4,0,MSG_MAXLEN-4);
        n=msgrcv(msgid,&msg,MSG_MAXLEN-4,pid,0);
        if(n<0) 
            ERR_EXIT("Server Close!");
        printf("rcv:type=%ld,byte=%d, msg=%s",msg.mtype,n-4,msg.mtext+4);
    }
    return 0;
}

int main(int argc,char *argv[])
{  
    int msgid;
    msgid= msgget(1234,0);
    if(msgid==-1)
        ERR_EXIT("msg_get");
   
    echo_cli(msgid);
    return 0;
}
