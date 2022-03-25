/* t_sysconf.c

   Demonstrate the use of sysconf() to retrieve system limits.
*/
#include "tlpi_hdr.h"

/* Print 'msg' plus sysconf() value for 'name' */
static void sysconf_print(const char *msg, int name)
{
    long lim;
    errno = 0;
    lim = sysconf(name);
    if (lim != -1)
    {
        printf("%s %ld\n", msg, lim);
    }
    else
    {
        if (errno == 0) /* Call succeeded, limit indeterminate */
            printf("%s (indeterminate)\n", msg);
        else /* Call failed */
            errExit("sysconf %s", msg);
    }
}

int main(int argc, char *argv[])
{
    sysconf_print("_SC_ARG_MAX:        ", _SC_ARG_MAX);
    sysconf_print("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
    sysconf_print("_SC_OPEN_MAX:       ", _SC_OPEN_MAX);
    sysconf_print("_SC_NGROUPS_MAX:    ", _SC_NGROUPS_MAX);
    sysconf_print("_SC_PAGESIZE:       ", _SC_PAGESIZE);
    sysconf_print("_SC_RTSIG_MAX:      ", _SC_RTSIG_MAX);
    exit(EXIT_SUCCESS);
}