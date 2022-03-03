/*
  Display the process env list
*/
#include "tlpi_hdr.h"

extern char **__environ; // or define _GNU_SOURCE to get it from <unistd.h>

int main(int argc, char *argv[])
{
    char **ep;

    for (ep = __environ; *ep != NULL; ep++)
        puts(*ep);

    exit(EXIT_SUCCESS);
}