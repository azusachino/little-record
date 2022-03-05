/**
 * A program that does nothing in particular, but the comments indicate which memory segments each type of variable is allocated in.
 */
#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>

char globBuf[65536];         // Uninitialized data segment
int primes[] = {2, 3, 5, 7}; // initialized data segment

static int square(int x) // allocated in frame for square
{
    int result;
    result = x * x;
    return result; // return value passed via register
}

static void doCalc(int val) // allocated in frame for doCalc
{
    printf("The square of %d is %d\n", val, square(val));
    if (val < 1000)
    {
        int t;

        t = val * val * val;
        printf("the cube of %d is %d\n", val, t);
    }
}

int main(int argc, char *argv[]) // allocated in frame for main()
{
    static int key = 9973;       // initialized data segment
    static char m_buf[10240000]; // 未初始化数据区间
    char *p;                     // main 栈帧

    p = malloc(1024); // points to memory in heap segment

    doCalc(key);

    exit(EXIT_SUCCESS);
}