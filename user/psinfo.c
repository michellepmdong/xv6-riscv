#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	struct ptable mypt;
	int count;
	int i;

	psinfo(mypt.procs, &count);
	printf("PID\tMEM\tNAME\n");

	for(i = 0; i < count; i++) {
		printf("%d\t%d\t%s\n", mypt.procs[i].pid, mypt.procs[i].mem, mypt.procs[i].name);
	}

	exit(0);
}