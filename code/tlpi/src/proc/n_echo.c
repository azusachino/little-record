#include "tlpi_hdr.h"

/**
 * print all args
 */
int main(int argc, char *argv[])
{
    int j = 0;
    for (j = 0; j < argc; j++)
    {
        printf("argv[%d] = %s\n", j, argv[j]);
    }

    exit(EXIT_SUCCESS);
}