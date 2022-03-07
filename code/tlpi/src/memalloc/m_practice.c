#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
    size_t sz = 1024;
    char *p;

    printf("initial program break: %10p\n", sbrk(0));

    p = malloc(sz);
    if (p == NULL)
    {
        printf("failed to malloc %d", sz);
        exit(EXIT_FAILURE);
    }

    printf("after malloc: %10p\n", sbrk(0));
    free(p);

    printf("after memory free: %10p\n", sbrk(0));

    void *y;
    y = alloca(sz);

    exit(EXIT_SUCCESS);
}