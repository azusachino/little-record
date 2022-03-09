#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct passwd *pwd;

    while ((pwd = getpwent()) != NULL)
    {
        printf("%-8s %5ld\n", pwd->pw_name, (long)pwd->pw_uid);
    }
    endpwent();
}