/* t_fpathconf.c

   Demonstrate the use of fpathconf() to retrieve the values of
   pathname-related limits.
*/
#include "tlpi_hdr.h"

static void fpathconf_print(const char *msg, int fd, int name)
{
    long lim;
    errno = 0;
    lim = fpathconf(fd, name);
    if (lim != -1)
    { /* Call succeeded, limit determinate */
        printf("%s %ld\n", msg, lim);
    }
    else
    {
        if (errno == 0) /* Call succeeded, limit indeterminate */
            printf("%s (indeterminate)\n", msg);
        else /* Call failed */
            errExit("fpathconf %s", msg);
    }
}

int main(int argc, char *argv[])
{
    fpathconf_print("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);
    fpathconf_print("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);
    fpathconf_print("_PC_PIPE_BUF: ", STDIN_FILENO, _PC_PIPE_BUF);
    exit(EXIT_SUCCESS);
}