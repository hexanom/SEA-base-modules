#ifndef SCHED_H
#define SCHED_H

#define STACK_SIZE 512 // ~10 func in stack depth and ~50 bytes per function
#define NULL 0

typedef void (*func_t) ( void);

struct ctx_s {
  unsigned int lr;
  unsigned int sp;
};

enum process_state {
  NEW,
  READY,
  RUNNING,
  BLOCKED,
  DONE
};

struct pcb_s {
  enum process_state state;
  struct ctx_s* ctx;
  struct pcb_s* next_pcb;
  void* args;
};

struct ctx_s* current_ctx;

void init_pcb(struct pcb_s* pcb, struct ctx_s* ctx);

int create_process(func_t f, void *args, unsigned int stack_size);

void start_sched();
void start_current_process();
void elect();
void __attribute__ ((naked)) ctx_switch();

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);
void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);

#endif
