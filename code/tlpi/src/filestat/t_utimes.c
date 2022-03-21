/* t_utimes.c

   Demonstrate the use of utimes(): set the last modification time on a file to
   be the same as the last access time, but tweak the microsecond components of
   the two timestamps.

   Usage: t_utimes file

   See also t_utime.c.
*/
#include <sys/stat.h>
#include <sys/time.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    struct stat sb;
    if (stat(argv[1], &sb) == -1) /* Retrieve current file times */
        errExit("stat");

    struct timeval tv[2];
    tv[0].tv_sec = sb.st_atime; /* Leave atime seconds unchanged */
    tv[0].tv_usec = 223344;     /* Change microseconds for atime */
    tv[1].tv_sec = sb.st_atime; /* mtime seconds == atime seconds */
    tv[1].tv_usec = 667788;     /* mtime microseconds */

    if (utimes(argv[1], tv) == -1)
        errExit("utimes");

    exit(EXIT_SUCCESS);
}