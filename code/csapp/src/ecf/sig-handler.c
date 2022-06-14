#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

typedef void (*handler_t)(int);

handler_t *signal_(int signum, handler_t *handler)
{
    struct sigaction action, old_action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    if (sigaction(signum, &action, &old_action) < 0)
    {
        fprintf(stderr, "sigaction %s\n", "error");
        exit(-1);
    }
    return (old_action.sa_handler);
}