#include "commheader.h"


int main(void)
{
    int msgid;
    msgid = msgget(1234, 0666 | IPC_CREAT);
    if (msgid == -1)
        ERR_EXIT("msgget");
    printf("msgget success\n");
    msgid = msgget(1234, 0);
    printf("msgid=%d\n", msgid);

    return 0;
}
