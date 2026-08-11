#include "commheader.h"

typedef struct stu{
    char name[4];
    int age;
}STU;

int main(int argc,char *argv[])
{  
    if(argc !=2)
    {
        fprintf(stderr,"Usage: %s <file>\n",argv[0]);
        exit(0);
    }
    int fd;
    fd = open(argv[1], O_RDWR);
    if(fd==-1)
        ERR_EXIT("Open File Error:");

    STU *ptr=NULL;
    //将文件映射到共享内存
    ptr = (STU*) mmap(NULL,sizeof(STU)*5,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == NULL)
        ERR_EXIT("mmap Error:");
    int i;
    //读取5个学生信息=>打印到控制台
    for(i=0;i<5;i++)
    {
        printf("name=%s,age=%d\n",(ptr+i)->name,(ptr+i)->age);
    }
    puts("read complete!");
    //解除共享内存的映射
    munmap(ptr,sizeof(STU)*5);
    puts("munmap Ok");
    return 0;
}
