#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

extern uint64 kmemfreecount();
extern int proccount();

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
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
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
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
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//added by rqx 2023/7/24
uint64
sys_trace(void)
{
   
   int mask;
   if (argint(0, &mask) < 0)
       return -1;
   myproc()->trace_mask = mask;
   return 0;
}

//added by rqx 2023/7/25
uint64
sys_sysinfo()
{
    uint64 addr; 
    struct sysinfo info;
    
    struct proc *p = myproc();
    
    info.freemem = kmemfreecount();      
    //printf("test sys_sysinfo 1 mem = %d\n",info.freemem);
    
    info.nproc = proccount();     
    //printf("test sys_sysinfo 2 nproc = %d\n",info.nproc);    
    
  
    if(argaddr(0, &addr) < 0)
       return -1;
  
  
    if(copyout(p->pagetable, addr, (char *)&info, sizeof(info))<0)
       return -1;
        
    //printf("test sys_sysinfo 3\n");
    return 0;
}


