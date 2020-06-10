struct stat;
struct rtcdate;
struct file;
struct pdata {
	int pid;
	int mem;
	char name[16];
};
struct ptable {
	struct pdata procs[64];
};

struct hdr {
  uint64 sz;
  uint64 code_data_sz;
  uint64 stack_sz;
  char name[16];
  uint magic;
  // uint64 ra;
  // uint64 epc;
  // uint64 sp;
  int tracing;
  struct trapframe *tf;
};

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int strace(void);
int psinfo(struct pdata*, int*);
int suspend(int, int);
int resume(char*);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
