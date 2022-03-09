#include <shadow.h>
#include <stdio.h>

int main() {
    struct spwd *sp;
    sp = getspent();
    printf("%-8s\n", sp->sp_namp);
    endspent();    
}