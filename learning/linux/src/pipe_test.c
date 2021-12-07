#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int ret = -1;
    int fd[2];
    pid_t pid;
    char buf[512] = {0};
    char *msg = "hello world";

    // create a pipe
    ret = pipe(fd);
    if (-1 == ret)
    {
        printf("failed to create pipe\n");
        return -1;
    }
    
    pid = fork();

    if (0 == pid)
    {                                         // 子进程
        close(fd[0]);                         // 关闭管道的读端
        ret = write(fd[1], msg, strlen(msg)); // 向管道写入数据
        exit(0);
    }
    else
    {                                        // 父进程
        close(fd[1]);                        // 关闭管道的写端
        ret = read(fd[0], buf, sizeof(buf)); // 从管道读取数据
        printf("parent read &d bytes data: %s\n", ret, buf);
    }
    return 0;
}