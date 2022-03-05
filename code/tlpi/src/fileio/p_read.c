#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "tlpi_hdr.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int fd;
    off_t orig;
    off_t offset = BUFFER_SIZE;
    char buf[BUFFER_SIZE];

    fd = open("copy.c", O_RDONLY);
    if (fd == -1)
    {
        errExit("open");
    }
    orig = lseek(fd, 0, SEEK_CUR);
    lseek(fd, offset, SEEK_SET);
    ssize_t sz = pread(fd, buf, 1024, offset);
    if (sz == -1)
    {
        errExit("pread");
    }
    printf("%s", buf);
    
    exit(EXIT_SUCCESS);
}