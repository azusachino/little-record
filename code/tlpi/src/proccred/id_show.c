/* idshow.c

   Display all user and group identifiers associated with a process.

   Note: This program uses Linux-specific calls and the Linux-specific
   file-system user and group IDs.
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>
#include "ugid_functions.h" /* username_from_id() & group_name_from_id() */
#include "tlpi_hdr.h"

#define SG_SIZE (NGROUPS_MAX + 1)

int main(int argc, char *argv[])
{
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t suppGroups[SG_SIZE];
    int numGroups, j;
    char *p;

    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");
    if (getresgid(&rgid, &egid, &sgid) == -1)
        errExit("getresgid");

    /* Attempts to change the file-system IDs are always ignored
       for unprivileged processes, but even so, the following
       calls return the current file-system IDs */

    fsuid = setfsuid(0);
    fsgid = setfsgid(0);

    printf("UID: ");
    p = username_from_id(ruid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)ruid);
    p = username_from_id(euid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)euid);
    p = username_from_id(suid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)suid);
    p = username_from_id(fsuid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsuid);
    printf("\n");

    printf("GID: ");
    p = group_name_from_id(rgid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)rgid);
    p = group_name_from_id(egid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)egid);
    p = group_name_from_id(sgid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)sgid);
    p = group_name_from_id(fsgid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsgid);
    printf("\n");

    numGroups = getgroups(SG_SIZE, suppGroups);
    if (numGroups == -1)
        errExit("getgroups");

    printf("Supplementary groups (%d): ", numGroups);
    for (j = 0; j < numGroups; j++)
    {
        p = group_name_from_id(suppGroups[j]);
        printf("%s (%ld) ", (p == NULL) ? "???" : p, (long)suppGroups[j]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}