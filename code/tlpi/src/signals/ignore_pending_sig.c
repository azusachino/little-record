/* ignore_pending_sig.c

   This program demonstrates what happens if we mark a pending signal
   (i.e., one that has been sent, but is currently blocked) as ignored.

   Usage: ignore_pending_sig

   Type Control-C (^C) to generate a SIGINT signal after the program prints
   its "sleeping" message (see below).
*/
#define _GNU_SOURCE /* Get strsignal() declaration from <string.h> */
#include <string.h>
#include <signal.h>
#include "signal_functions.h" /* Declaration of printSigset() */
#include "tlpi_hdr.h"

static void handler(int sig)
{
    /* UNSAFE: This handler uses non-async-signal-safe functions
   (printf(), strsignal(), fflush(); see Section 21.1.2) */

    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    fflush(NULL);
}

int main(int argc, char *argv[])
{
    const int numSecs = 5;

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        errExit("sigaction");
    }

    sigset_t blocked;
    sigemptyset(&blocked);
    sigaddset(&blocked, SIGINT);

    if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
    {
        errExit("sigprocmask");
    }

    printf("BLOCKING SIGINT for %d seconds\n", numSecs);
    sleep(numSecs);

    /* Display mask of pending signals */

    sigset_t pending;
    if (sigpending(&pending) == -1)
        errExit("sigpending");
    printf("PENDING signals are: \n");
    printSigset(stdout, "\t\t", &pending);

    /* Now ignore SIGINT */

    sleep(2);
    printf("Ignoring SIGINT\n");
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        errExit("signal");

    /* Redisplay mask of pending signals */

    if (sigpending(&pending) == -1)
        errExit("sigpending");
    if (sigismember(&pending, SIGINT))
    {
        printf("SIGINT is now pending\n");
    }
    else
    {
        printf("PENDING signals are: \n");
        printSigset(stdout, "\t\t", &pending);
    }
    sleep(2);

    /* Reestablish SIGINT handler */

    printf("Reestablishing handler for SIGINT\n");
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    sleep(2);

    /* And unblock SIGINT */

    printf("UNBLOCKING SIGINT\n");
    sigemptyset(&blocked);
    if (sigprocmask(SIG_SETMASK, &blocked, NULL) == -1)
        errExit("sigprocmask");

    exit(EXIT_SUCCESS);
}