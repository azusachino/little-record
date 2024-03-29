#define _GNU_SOURCE
#include "sys/capability.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "stdio.h"
#include "sched.h"
#include "signal.h"
#include "unistd.h"

#define STACK_SIZE (1024*1024)
static char child_stack[STACK_SIZE];

void set_uid_map(pid_t pid, int inside_id, int outside_id, int length) {
    char path[256];
    sprintf(path, "/proc/%d/uid_map", getpid());
    FILE* uid_map = fopen(path, "w");
    fprintf(uid_map, "%d %d %d", inside_id, outside_id, length);
    fclose(uid_map);
}

void set_gid_map(pid_t pid, int inside_id, int outside_id, int length) {
    char path[256];
    sprintf(path, "/proc/%d/gid_map", getpid());
    FILE* gid_map = fopen(path, "w");
    fprintf(gid_map, "%d %d %d", inside_id, outside_id, length);
    fclose(gid_map);
}

char* const child_args[] = {"/bin/bash", NULL};

int child_main(void *args) {
    printf("in child process!\n");
    cap_t caps;
    set_uid_map(getpid(), 0, 1000, 1);
    set_gid_map(getpid(), 0, 1000, 1);
    printf("eUID = %1d; eGID = %1d", (long)geteuid(), (long)getegid());
    caps = cap_get_proc();
    printf("capacilities: %s\n", cap_to_text(caps, NULL));
    sethostname("NewNamespace", 12);
    execv(child_args[0], child_args);
    return 1;
}

int main() {
    printf("program starts! \n");
    int child_pid = clone(child_main, child_stack+STACK_SIZE, CLONE_NEWUSER | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("program exited! \n");
    return 0;
}