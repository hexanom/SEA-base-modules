#include "sched.h"
#include "phyAlloc.h"

struct pcb_s* first_pcb = NULL;
struct pcb_s* last_pcb = NULL;
struct pcb_s* current_process = NULL;

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size) {
  ctx->lr = (unsigned int)f;
  ctx->sp = ((unsigned int)phyAlloc_alloc(STACK_SIZE)) + STACK_SIZE - 1 - 13;
}

void init_pcb(struct pcb_s* pcb, struct ctx_s* ctx) {
  pcb->ctx = ctx;
  pcb->state = NEW;
  
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
  ((func_t)(current_process->ctx->lr))();
}

void elect() {
  current_process = current_process->next_pcb;
}

void start_sched() {
  current_process = first_pcb;
}

void __attribute__ ((naked)) ctx_switch() {

  // 1. Save context
  __asm("push {r0-r12}");
  __asm("mov %0, sp" : "=r"(current_process->ctx->sp));
  __asm("mov %0, lr" : "=r"(current_process->ctx->lr));

  // 2. Request new process
  elect();

  // 3. Restore context
  __asm("mov sp, %0" : : "r"(current_process->ctx->sp));
  __asm("mov lr, %0" : : "r"(current_process->ctx->lr));
  __asm("pop {r0-r12}");
  __asm("bx lr");
}

int create_process(func_t f, void *args, unsigned int stack_size) {
  struct ctx_s* ctx = phyAlloc_alloc(sizeof(struct ctx_s));
  struct pcb_s* pcb = phyAlloc_alloc(sizeof(struct pcb_s));
  
  init_ctx(ctx, f, stack_size);
  init_pcb(pcb, ctx);
  
  return 0; // ?
}
