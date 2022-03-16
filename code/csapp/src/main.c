#include <stdio.h>

typedef unsigned char *pointer;

void show_bytes(pointer start, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
    {
        printf("%p\t0x%.2x\n", start + i, start[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int a = 15213;
    show_bytes((pointer)&a, sizeof(int));
    printf("Hello CSAPP\n");
}