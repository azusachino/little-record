#ifndef UGID_FUNCTIONS_H
#define UGID_FUNCTIONS_H

#include "tlpi_hdr.h"

char *username_from_id(uid_t);

uid_t user_id_from_name(const char *);

char *group_name_from_id(gid_t);

gid_t group_id_from_name(const char *);

#endif