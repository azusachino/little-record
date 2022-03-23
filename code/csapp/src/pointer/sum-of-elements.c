#include <stdio.h>

// a is int* a
int sum(int a[])
{
    int i, sum;
    int size = sizeof(a) / sizeof(a[0]);

    printf("[sum] sizeof(a) = %d, sizeof(a[i]) = %d\n", sizeof(a), sizeof(a[0]));
    for (i = 0; i < size; i++)
    {
        sum += a[i];
    }
    return sum;
}

int sum2(int *a, int size)
{
    int i, sum;
    for (i = 0; i < size; i++)
    {
        sum += a[i];
    }
    return sum;
}

int main()
{
    int a[] = {1, 2, 3, 4, 5};
    printf("[main] sizeof(a) = %d, sizeof(a[i]) = %d\n", sizeof(a), sizeof(a[0]));
    int total = sum(a);
    int size = sizeof(a) / sizeof(a[0]);
    int total2 = sum2(a, size);

    printf("sum = %d\n, %d", total, total2);
}