#include <stdio.h>

int Add(int a, int b)
{
    return a + b;
}

int main()
{
    int c;
    int (*p)(int, int);
    // or p = Add
    p = &Add;

    c = (*p)(1, 2);
    printf("%d\n", c);
}