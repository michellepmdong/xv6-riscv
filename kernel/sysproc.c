#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_trace_on(void)
{
  myproc()->tracing = 1;
  return myproc()->tracing;
}

uint64
sys_psinfo(void)
{
  uint64 pt_p;
  uint64 count_p;
  int ptable_rv;
  int count_rv;

  ptable_rv = argaddr(0, &pt_p);
  count_rv = argaddr(1, &count_p);

  if(myproc()->tracing == 1) {
    printf("[%d] sys_psinfo(%p, %p)\n", myproc()->pid, pt_p, count_p);
  }

  if ((ptable_rv < 0) || (count_rv < 0)) {
    return -1;
  }
  return psinfo(pt_p, count_p);
}

uint64
sys_exit(void)
{
  int n;
  int rv;

  rv = argint(0, &n);

  if(myproc()->tracing == 1) {
    printf("[%d] sys_exit(%d)\n", myproc()->pid, n);
  }

  if(rv < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  if(myproc()->tracing == 1) {
    printf("[%d] sys_getpid()\n", myproc()->pid);
  }
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  if(myproc()->tracing == 1) {
    printf("[%d] sys_fork()\n", myproc()->pid);
  }
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  int rv;

  rv = argaddr(0, &p);

  if(myproc()->tracing == 1) {
    printf("[%d] sys_wait(%p)\n", myproc()->pid, p);
  }

  if(rv < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;
  int rv;

  rv = argint(0, &n);

  if(myproc()->tracing == 1) {
    printf("[%d] sys_sbrk(%d)\n", myproc()->pid, n);
  }

  if(rv < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;

  if(myproc()->tracing == 1) {
    printf("[%d] sys_sleep(%d, %p)\n", myproc()->pid, n, (void*) &tickslock);
  }

  while(ticks - ticks0 < n){ //time elapsed hasn't been the full sleep cycle
    if(myproc()->killed){ //if you got killed, just release the time lock
      release(&tickslock);
      return -1;
    }

    sleep(&ticks, &tickslock); //else keep sleeping
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;
  int rv;

  rv = argint(0, &pid);

  if(myproc()->tracing == 1) {
    printf("[%d] sys_kill(%d)\n", myproc()->pid, pid);
  }

  if(rv < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  if(myproc()->tracing == 1) {
    printf("[%d] sys_uptime()\n", myproc()->pid);
  }
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
