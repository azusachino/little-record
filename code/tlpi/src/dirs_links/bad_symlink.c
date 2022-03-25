/* bad_symlink.c

   The following code demonstrates a mistake in using symlink(): the link
   is created with an incorrect relative path, and the subsequent chmod()
   call fails. Note: symbolic links are interpreted relative to the directory
   in which they reside, not the current directory of the process.
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (mkdir("test", S_IRUSR | S_IWUSR | S_IXUSR) == -1)
        errExit("mkdir");
    if (chdir("test") == -1)
        errExit("chdir");

    int fd = open("myfile", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");
    if (close(fd) == -1)
        errExit("close");

    if (symlink("myfile", "../mylink") == -1)
        errExit("symlink");
    if (chmod("../mylink", S_IRUSR) == -1)
        errExit("chmod");

    exit(EXIT_SUCCESS);
}