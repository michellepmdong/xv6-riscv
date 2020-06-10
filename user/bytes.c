#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void
print_bytes(char *label, char *mem, int n)
{
    int i;
    printf("bytes[%s] = ", label);
    for (i = 0; i < n; i++) {
        printf("%x ", mem[i]);
    }
    printf("\n");
}


int
main(int argc, char *argv[])
{
    char *filename;
    int offset;
    int count;
    int fd;
    char buf[512];
    int i;

    if (argc != 4) {
        printf("usage: bytes <filename> <offset> <count>\n");
        exit(0);
    }

    filename = argv[1];
    offset = atoi(argv[2]);
    count = atoi(argv[3]);
    fd = open(filename, O_RDONLY);

    /* get to offset */
    for (i = 0; i < offset; i++) {
        read(fd, buf, 1);
    }

    /* get bytes */
    read(fd, buf, count);

    print_bytes("bytes", buf, count);

    exit(0);

}