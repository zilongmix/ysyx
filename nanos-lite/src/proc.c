#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB* current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void* arg) {
  int j = 1;
  while (1) {
    if (j==1000)
    {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", arg, j);
    j=0;
    }
    j++;
    yield();
  }
}




char* pal_argv[] = {
  NULL
};

char* pal_envp[] = {
  // "home=pwd",
  // "ARCH=riscv",
  // "ARCH=riscv1",
  // "ARCH=riscv2",
NULL
};

void init_proc() {
  Log("Initializing processes...");

  context_kload(&pcb[0], hello_fun, "first");
  context_uload(&pcb[1], "/bin/nterm", pal_argv, pal_envp);


  switch_boot_pcb();



  // // load program here
  // naive_uload(NULL, "/bin/menu");
}

Context* schedule(Context* prev) {
  // save the context pointer
  current->cp = prev;

  // always select pcb[0] as the new process
  // current = &pcb[0];
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);

  // then return the new context
  return current->cp;

}
