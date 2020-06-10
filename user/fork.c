#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int id;
    int x = 3;

    fprintf(1, "Parent: x = %d\n", x);

    id = fork();

    if (id == 0) {
        /* we are in the child */
        fprintf(1, "Child: id = %d\n", id);
        fprintf(1, "Child PID = %d\n", getpid());
        fprintf(1, "Child: x = %d\n", x);
        x = 99;
        fprintf(1, "Child: x = %d\n", x);
        fprintf(1, "Child: &x = %x\n", &x);
        exit(0);
    } else {
        /* we are in the parent */
        fprintf(1, "Parent PID = %d\n", getpid());
        fprintf(1, "Parent: id = %d\n", id);
        fprintf(1, "Parent: wait for child\n");
        id = wait(0);
        fprintf(1, "Parent: child terminated pid = %d\n", id);
        fprintf(1, "Parent: x = %d\n", x);
        fprintf(1, "Parent: &x = %x\n", &x);
    }

    exit(0);
}

