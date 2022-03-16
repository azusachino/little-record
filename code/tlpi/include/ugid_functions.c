/* ugid_functions.c

   Implements a set of functions that convert user/group names to user/group IDs
   and vice versa.
*/
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include "ugid_functions.h"

char *username_from_id(uid_t uid)
{
    struct passwd *pwd;
    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

uid_t user_id_from_name(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *end_ptr;

    if (name == NULL || *name == '\0')
    {
        return -1;
    }
    u = strtol(name, &end_ptr, 10);
    if (*end_ptr == '\0')
    {
        return u;
    }
    pwd = getpwnam(name);
    if (pwd == NULL)
    {
        return -1;
    }
    return pwd->pw_uid;
}

char *group_name_from_id(gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}

gid_t group_id_from_name(const char *name)
{
    struct group *grp;
    gid_t g;
    char *endptr;

    if (name == NULL || *name == '\0') /* On NULL or empty string */
        return -1;                     /* return an error */

    g = strtol(name, &endptr, 10); /* As a convenience to caller */
    if (*endptr == '\0')           /* allow a numeric string */
        return g;

    grp = getgrnam(name);
    if (grp == NULL)
        return -1;

    return grp->gr_gid;
}