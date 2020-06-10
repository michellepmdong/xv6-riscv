#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int id;
	id = fork();

	if (id == 0) {
		//TODO: argv arr up to n
		strace();
		exec(argv[1], argv+1); //&argv[1]???
	}
	else {
		id = wait(0);
	}
	exit(0);
}