#ifndef SCHED_H
#define SCHED_H

#define STACK_SIZE 512 // ~10 func in stack depth and ~50 bytes per function

typedef void (*func_t) ( void);

struct ctx_s {
  unsigned int lr;
  unsigned int sp;
};

struct ctx_s* current_ctx;

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);
void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);

#endif
