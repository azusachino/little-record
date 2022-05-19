#ifdef RUNTIME
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// malloc wrapper function
void *malloc(size_t size)
{
    void *(*mallocp)(size_t size);
    char *error;

    // get address of libc malloc
    mallocp = dlsym(RTLD_NEXT, "malloc");
    if ((error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(EXIT_FAILURE);
    }

    // call libc malloc
    char *ptr = mallocp(size);
    printf("malloc(%d) = %p\n", (int)size, ptr);
    return ptr;
}

// free wrapper function
void free(void *ptr)
{
    void (*freep)(void *) = NULL;
    char *error;

    if (!ptr)
        return;
    // get address of libc free
    freep = dlsym(RTLD_NEXT, "free");
    if ((error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(EXIT_FAILURE);
    }
    // call libc free
    freep(ptr);
    printf("free(%p)\n", ptr);
}

#endif