#include <stdio.h>

int main(void)
{
    int x = 1025;

    int *p = &x;
    printf("the size of int is %d\n", sizeof(int));
    printf("addr %p, val %d\n", p, *p);
    printf("addr %p, val %d\n", p + 1, *(p + 1));

    char *p0 = (char *)p;
    printf("the size of char is %d\n", sizeof(int));
    printf("addr %p, val %d\n", p0, *p0);           // 1
    printf("addr %p, val %d\n", p0 + 1, *(p0 + 1)); // 40
    return 0;
}

// Addr: &A[i] or A + i
// Value: A[i] or *(A+i)
void show_array()
{
    int A[5] = {2, 4, 5, 8, 1};

    printf("%d\n", &A[0]);
    printf("%d\n", A);
}