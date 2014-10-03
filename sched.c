#include "sched.h"
#include "phyAlloc.h"

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size) {
  ctx->lr = (unsigned int)f;
  ctx->sp = ((unsigned int)phyAlloc_alloc(STACK_SIZE)) + STACK_SIZE - 1 - 13;
}

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx) {

  // 1. Save context
  __asm("push {r0-r12}");
  __asm("mov %0, sp" : "=r"(current_ctx->sp));
  __asm("mov %0, lr" : "=r"(current_ctx->lr));

  // 2. Change current context
  current_ctx = ctx;

  // 3. Restore context
  __asm("mov sp, %0" : : "r"(current_ctx->sp));
  __asm("mov lr, %0" : : "r"(current_ctx->lr));
  __asm("pop {r0-r12}");
  __asm("bx lr");
}
