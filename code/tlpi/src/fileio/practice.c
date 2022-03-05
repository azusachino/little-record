#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int flags, accessMode;
    char *filename = "hello.txt";
    int fd;

    fd = open(filename, O_RDONLY | O_SYNC);
    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
    {
        exit(EXIT_FAILURE);
    }
    printf("%d\n", flags);
    if (flags & O_SYNC)
    {
        printf("writes are sync\n");
    }
    accessMode = flags & O_ACCMODE;
    if (accessMode == O_WRONLY || accessMode == O_RDWR)
    {
        printf("file is writable");
    }
    exit(EXIT_SUCCESS);
}
