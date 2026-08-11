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
    fd = open(argv[1],O_CREAT | O_RDWR | O_TRUNC,0666);
    if(fd==-1)
        ERR_EXIT("Open File Error:");
    lseek(fd,sizeof(STU)*5-1,SEEK_SET);//SEEK_SET从头开始定位
    write(fd,"",1);

    STU *ptr=NULL;
    //将文件映射到共享内存
    ptr = (STU*) mmap(NULL,sizeof(STU)*5,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == NULL)
        ERR_EXIT("mmap Error:");
    char ch='a';
    int i;
    //写个5个学生信息=>共享内存
    for(i=0;i<5;i++)
    {
        memcpy((ptr+i)->name,&ch,1);
        (ptr+i)->age=20+i;
        ch++;
    }
    puts("initialize Ok!");
    //解除共享内存的映射
    munmap(ptr,sizeof(STU)*5);
    puts("munmap Ok");
    return 0;
}
