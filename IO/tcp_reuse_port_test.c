#include <sys/eventfd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h> //DIR need
#include <unistd.h> //archlinux need
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int close_all_fd(int pid) {

    DIR *dir;
    struct dirent *entry, _entry;
    int retval, rewind, fd;

    char spath[30] = { 0 };
    sprintf(spath, "/proc/%d/fd", pid);
    printf("spath = <%s>\n", spath);

    dir = opendir(spath);
    if (dir == NULL)
        return -1;

    rewind = 0;
    while (1) {
        retval = readdir_r(dir, &_entry, &entry);
        if (retval != 0) {
            printf("fail readdir_r() retval == %d\n", retval);
            retval = -1;
            break;
        }
        if (entry == NULL) {
            if (!rewind)
                break;
            rewinddir(dir);
            rewind = 0;
            continue;
        }
        if (entry->d_name[0] == '.')
            continue;
        //遍历的时候分析下描述符对应的软连接是不是指向一个socket，以免错杀无辜的描述符
        {
            char link_name[1024] = {0};
            int ret_link = 0;
            ret_link = readlink(entry->d_name, link_name, 1024);
            if ((ret_link == 0) && (strcmp(link_name, "socket:") == 0)) {
                continue;
            }
        }
        fd = atoi(entry->d_name);
        if (dirfd(dir) == fd)
            continue;
        if (fd < 3) /***jump root dev**/
            continue;
        printf("will close fd = %d\n", fd);
        retval = close(fd);
        if (retval != 0)
            break;
        rewind = 1;
    }
    closedir(dir);
    return retval;
}

/* 创建socket */
int create_socket_and_bind(const char *ip, const int port_number)
{
    struct sockaddr_in server_addr;
    int sockfd, reuse = 1;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);

    if (inet_pton(PF_INET, ip, &server_addr.sin_addr) == -1)
        handle_error("inet_pton() error");

    if ((sockfd = socket(PF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0)) == -1)
        handle_error("socket() error");

    /* 设置复用socket port */
    //if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    //    handle_error("setsockopt() SO_REUSEADDR error");

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1)
            handle_error("setsockopt() SO_REUSE PORT error");

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("bind() error");

    if (listen(sockfd, 5) == -1)
        handle_error("listen() error");
    printf("tid %ld, bind port %d.\n", gettid(), port_number);
    return sockfd;
}

int main(int argc, char *argv[]) {
    int fd, sub_fd;
    if (argc < 3) {
        fprintf(stderr, "usage: %s 127.0.0.1 8000\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    fd = create_socket_and_bind(argv[1], atoi(argv[2]));
    printf("fd == %d\n", fd);

    int pid = getpid();
    switch (fork()) {
    case 0:
        // close_all_fd(pid);
        close(fd);
        sub_fd = create_socket_and_bind(argv[1], atoi(argv[2]));
        printf("sub_fd == %d\n", sub_fd);
        sleep(30);
        printf("Child completed write loop\n");
        exit(EXIT_SUCCESS);
    default:
        printf("main..\n");
        sleep(1);
        break;
    case -1:
        handle_error("fork");
    }
    getchar();
    printf("exit main thread!\n");
    return 0;
}
