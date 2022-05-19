#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int main()
{

    void *handle;
    void (*addVec)(int *, int *, int *, int);
    char *error;

    // dynamically load the shared library containing addVec()
    handle = dlopen("./libvector.so", RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // get a pointer to the addVec() function
    addVec = dlsym(handle, "addVec");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    // call the func
    addVec(x, y, z, 2);
    printf("z = [%d %d]\n", z[0], z[1]);

    // unload the shared library
    if (dlclose(handle) < 0)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    return 0;
}