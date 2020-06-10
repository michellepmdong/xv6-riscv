#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
	int pid;
	char* filename;
	int fd;

	pid = atoi(argv[1]);
	filename = argv[2];

	fd = open(filename, O_CREATE | O_RDWR);

	suspend(fd, pid);

	close(fd); //not closing results in scause 2 reserved?

	exit(0);
}