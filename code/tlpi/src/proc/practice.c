#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void my_setenv(int argc, char *argv[])
{
    // we need args like my_setenv a b
    if (argc != 3)
    {
        printf("args count error");
        exit(EXIT_FAILURE);
    }
    char *r;
    sprintf(r, "%s=%s", argv[1], argv[2]);
    if (0 != putenv(r))
    {
        printf("putenv failed");
        exit(EXIT_FAILURE);
    }

    // check success
    char *ret = getenv(argv[1]);
    printf("ret %s", ret);
}

void my_unsetenv(int argc, char *argv[])
{
}

int main(int argc, char *argv[])
{
    my_setenv(argc, argv);
    exit(EXIT_SUCCESS);
}