#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int id;
    char *argv[4];
    argv[0] = "hello_xv6";
    argv[1] = 0;

    id = fork();

    if (id == 0) {
        /* we are in the child */
        exec(argv[0], argv);
        fprintf(1, "Child: WE DON'T SEE THIS\n");
        exit(0);
    } else {
        /* we are in the parent */
        id = wait(0);
        fprintf(1, "Parent: child terminated\n");
    }

    exit(0);
}