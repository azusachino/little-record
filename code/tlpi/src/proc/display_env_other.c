#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>

extern char** __environ;

int main() {
    char **ep;
    for (ep = __environ; ep != NULL; ep++) {
        puts(*ep);
    }
    exit(EXIT_SUCCESS);
}