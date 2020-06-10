#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int id;
	char* filename;

	id = fork();

	filename = argv[1];

    if (id == 0) {
	    /* we are in the child */
	    resume(filename);
    } else {
        /* we are in the parent */
        id = wait(0);
        fprintf(1, "Parent: child terminated\n");
    }

	exit(0);
}