#include <stdio.h>

int main()
{

    char C[10];

    C[0] = '1';

    // as NULL, in ascii is 0
    C[4] = '\0';
    C[6] = '2';

    C[10] = '\0';

    printf("%s\n", C);

    char c[20] = "hello"; // stack
    char *c2 = "hello";   // bss

    // b[i][j] == *(b[i] + j) = *(*(b+i)+j)
}