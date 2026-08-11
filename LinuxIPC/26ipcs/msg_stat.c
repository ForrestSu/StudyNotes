#include "commheader.h"

int main(void)
{
    int msgid;
    msgid = msgget(1234, 0);
    if (msgid == -1)
        ERR_EXIT("msgget");
    printf("msgget success\n");
    printf("msgid=%d\n", msgid);

    struct msqid_ds buf;
    msgctl(msgid, IPC_STAT, &buf);
    printf("mode : %o\n", buf.msg_perm.mode);
    printf("bytes=%ld\n", buf.__msg_cbytes);
    printf("number=%d\n", (int)buf.msg_qnum);
    printf("msgmnb=%d\n", (int)buf.msg_qbytes);
    return 0;
}

