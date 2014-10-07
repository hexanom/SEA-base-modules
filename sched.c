#include "sched.h"
#include "phyAlloc.h"

struct pcb_s* first_pcb = NULL;
struct pcb_s* last_pcb = NULL;
struct pcb_s* current_process = NULL;

void init_pcb(struct pcb_s* pcb, func_t entry_point, void* args) {
  pcb->lr = (unsigned int)&start_current_process;
  pcb->sp = ((unsigned int)phyAlloc_alloc(STACK_SIZE));
  pcb->sp += STACK_SIZE;
  pcb->sp -= 4; 
  pcb->sp -= 13*4;
  
  pcb->entry_point = entry_point;
  pcb->args = args;
  pcb->state = NEW;
}

void add_pcb(struct pcb_s* pcb) {
  if(last_pcb != NULL) {
    last_pcb->next_pcb = pcb;
  }
  else {
    first_pcb = pcb;
  }
  pcb->next_pcb = first_pcb;
  last_pcb = pcb;
}

void start_current_process() {
  current_process->state = RUNNING;
  current_process->entry_point(current_process->args);
}

void elect() {
  do {
    current_process = current_process->next_pcb;
  } while(current_process->state != READY && current_process->state != NEW);
}

void start_sched() {
  struct pcb_s* kmain_pcb = phyAlloc_alloc(sizeof(struct pcb_s));
  init_pcb(kmain_pcb, NULL, NULL);
  kmain_pcb->next_pcb = first_pcb;
  current_process = kmain_pcb;
}

void __attribute__ ((naked)) ctx_switch() {

  __asm("push {r0-r12}");
  __asm("mov %0, sp" : "=r"(current_process->sp));
  __asm("mov %0, lr" : "=r"(current_process->lr));
  current_process->state = READY;
  
  elect();
  
  current_process->state = RUNNING;
  __asm("mov sp, %0" : : "r"(current_process->sp));
  __asm("mov lr, %0" : : "r"(current_process->lr));
  __asm("pop {r0-r12}");
  __asm("bx lr");
  
}

int create_process(func_t f, void *args, unsigned int stack_size) {
  struct pcb_s* pcb = phyAlloc_alloc(sizeof(struct pcb_s));
  
  init_pcb(pcb, f, args);
  add_pcb(pcb);
  
  return 0; // ?
}
