#include "commheader.h"
#include <pthread.h>

void* thread_route(void * arg)
{
    int i;
    for(i=0;i<20;i++)
    {
         printf("B");
         fflush(stdout);
		 usleep(20);
		 if(i>3)
		 {
		  //   pthread_exit("ABC");
		 }
	}
	sleep(3);//sleep 3秒
	return 0;
}
int main()
{
    pthread_t tid;
    int ret;
    int i;
    ret = pthread_create(&tid,NULL,thread_route,NULL);
    if(ret!=0)
    {
        fprintf(stderr, "pthread_create:%s\n",strerror(ret));
        exit(EXIT_FAILURE);
    }
    for(i=0; i<20;i++)
    {
        printf("A");
        fflush(stdout);
        usleep(20);
    }
    //sleep(1);//等待创建的线程结束
    void *value;
	if((ret = pthread_join(tid,&value))!=0)
    {
       fprintf(stderr, "pthread_join:%s\n",strerror(ret));
       exit(EXIT_FAILURE);
    }

    printf("\nNew Thread return msg=%s\n",(char*)value);
    return 0;
}
