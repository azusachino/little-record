#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void write_append(int argc, char *argv[])
{
    // we only need anthoer file name
    if (argc != 2)
    {
        printf("please only provide one filename");
        exit(EXIT_FAILURE);
    }
    int fd;
    // open with append and wr
    fd = open(argv[1], O_APPEND | O_WRONLY);
    if (fd == -1)
    {
        printf("failed to open file: %s", argv[1]);
        exit(EXIT_FAILURE);
    }
    // set offset to start
    lseek(fd, 0, SEEK_SET);
    char *op = "oops";
    write(fd, op, sizeof(op));

    exit(EXIT_SUCCESS);
}

void test_write(int argc, char *argv[])
{
    int fd1, fd2, fd3;
    fd1 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fd2 = dup(fd1);
    fd3 = open(argv[1], O_RDWR);

    write(fd1, "Hello,", 6);
    write(fd2, "world", 6);
    lseek(fd2, 0, SEEK_SET);

    write(fd1, "HELLO,", 6);
    write(fd3, "Gidday", 6);
}

int main(int argc, char *argv[])
{
    test_write(argc, argv);

    exit(EXIT_SUCCESS);
}
