/*
  Demostrate modification of the process environment list.
  Usage: modify_env name=value ...
  Note: some UNIX implementations do not provide clearenv(), setenv() and unsetenv()
*/
#define _GNU_SOURCE // Get various declarations from <stdlib.h>
#include <stdlib.h>
#include <tlpi_hdr.h>

extern char **__environ;

int main(int argc, char *argv[])
{
    int j;
    char **ep;

    clearenv(); // Erase entire environment

    for (j = 1; j < argc; ++j)
    {
        if (putenv(argv[j]) != 0)
        {
            errExit("putenv: %s", argv[j]);
        }
    }

    // add a definition for GREET if one does not already exist
    if (setenv("GREET", "hello world", 0) == -1)
    {
        errExit("setenv");
    }

    // remove any existing definition of BYE
    unsetenv("BYE");

    for (ep = __environ; ep != NULL; ++ep)
    {
        puts(*ep);
    }

    exit(EXIT_SUCCESS);
}