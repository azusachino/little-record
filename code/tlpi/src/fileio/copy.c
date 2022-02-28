#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

/* copy.c

   Copy the file named argv[1] to a new file named in argv[2].
*/
int main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;

    mode_t filePerms;
    ssize_t numRead;
    char buf[BUFFER_SIZE];

    if (argc != 3 || strcpm(argv[1], "--help") == 0)
    {
        usageErr("%s old-file new-file\n", argv[0]);
    }

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
    {
        errExit("opening file %s", argv[1]);
    }
    /* create, readonly, truncate */
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    /* rw-rw-rw */
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1)
    {
        errExit("opening file %s", argv[2]);
    }

    /* Transfer data until we encounter end of input or an error */
    while ((numRead = read(inputFd, buf, BUFFER_SIZE)) > 0)
    {
        if (write(outputFd, buf, numRead) != numRead)
        {
            fatal("write() returned error or partial write occurred");
        }
    }
    if (numRead == -1)
    {
        errExit("read failed");
    }

    if (close(inputFd) == -1)
    {
        errExit("close input fd");
    }
    if (close(outputFd) == -1)
    {
        errExit("close output fd");
    }

    exit(EXIT_SUCCESS);
}