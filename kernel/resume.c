#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

static int
loadseg(pagetable_t pagetable, uint64 va, struct inode *ip, uint offset, uint sz)
{
  uint i, n;
  uint64 pa;

  if((va % PGSIZE) != 0)
    panic("loadseg: va must be page aligned");

  for(i = 0; i < sz; i += PGSIZE){
    pa = walkaddr(pagetable, va + i);
    if(pa == 0)
      panic("loadseg: address should exist");
    if(sz - i < PGSIZE)
      n = sz - i;
    else
      n = PGSIZE;
    if(readi(ip, 0, (uint64)pa, offset+i, n) != n)
      return -1;
  }
  
  return 0;
}

int
resume(char *path)
{
  uint64 sz;
  struct hdr h;
  struct trapframe tf;
  struct inode *ip;
  pagetable_t pagetable = 0, oldpagetable;
  struct proc *p = myproc();

  begin_op();

  if((ip = namei(path)) == 0){
    end_op();
    return -1;
  }
  ilock(ip);

  // Check ELF header
  if(readi(ip, 0, (uint64)&h, 0, sizeof(h)) != sizeof(h))
    goto bad;
  if(readi(ip, 0, (uint64)&tf, sizeof(h), sizeof(tf)) != sizeof(tf))
    goto bad;
  if(h.magic != SUS_MAGIC)
    goto bad;
  if((pagetable = proc_pagetable(p)) == 0)
    goto bad;

  // Load program into memory.
  sz = 0;
  if((sz = uvmalloc(pagetable, sz, h.code_data_sz)) == 0)
    goto bad;
  if(loadseg(pagetable, 0, ip, sizeof(h)+sizeof(tf), h.code_data_sz) < 0)
    goto bad;

  p = myproc();
  uint64 oldsz = p->sz;

  // Allocate two pages at the next page boundary.
  // Use the second as the user stack.
  sz = PGROUNDUP(sz);
  if((sz = uvmalloc(pagetable, sz, sz + 2*PGSIZE)) == 0)
    goto bad;
  uvmclear(pagetable, sz-2*PGSIZE);
  if(loadseg(pagetable, sz-PGSIZE, ip, sizeof(h) + sizeof(tf) + h.code_data_sz, PGSIZE) < 0)
    goto bad;
  iunlockput(ip); // needed to move this after the last loadseg to the inode
  end_op();
  ip = 0;

  safestrcpy(p->name, h.name, sizeof(p->name));
    
  // Commit to the user image.
  oldpagetable = p->pagetable;
  p->pagetable = pagetable;
  p->sz = sz;

  // TODO
  // p->tf->epc = tf.epc;  // initial program counter = main TODO
  // p->tf->ra = tf.ra;
  // p->tf->sp = tf.sp; // initial stack pointer
  // p->tf->a0 = tf.a0;
  // p->tf->a1 = tf.a1;
  // p->tf->a2 = tf.a2;
  // p->tf->a3 = tf.a3;
  // p->tf->a4 = tf.a4;
  // p->tf->a5 = tf.a5;
  // p->tf->a6 = tf.a6;
  // p->tf->a7 = tf.a7;
  *(p->trapframe) = tf; // got an access scause if I copied tf ptr from hdr

  proc_freepagetable(oldpagetable, oldsz);
  return 0; // this ends up in a0, the first argument to main(argc, argv)

 bad:
  if(pagetable)
    proc_freepagetable(pagetable, sz);
  if(ip){
    iunlockput(ip);
    end_op();
  }
  return -1;
}

