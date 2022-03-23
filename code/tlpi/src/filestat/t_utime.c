/* t_utime.c

   Demonstrate the use of utime(): set the last modification time on a file to
   be the same as the last access time.

   Usage: t_utime file

   See also t_utimes.c.
*/
#include <sys/stat.h>
#include <utime.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    char *pathname = argv[1];

    struct stat sb;
    if (stat(pathname, &sb) == -1) /* Retrieve current file times */
        errExit("stat");

    struct utimbuf utb;
    utb.actime = sb.st_atime;        /* Leave access time unchanged */
    utb.modtime = sb.st_atime;       /* Make modify time same as access time */
    if (utime(pathname, &utb) == -1) /* Update file times */
        errExit("utime");

    exit(EXIT_SUCCESS);
}