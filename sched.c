#include "sched.h"
#include "phyAlloc.h"

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size) {
  (*ctx).pc = f;
  (*ctx).sp = phyAlloc_alloc(STACK_SIZE);
}

void __attribute__ ((naked)) switch_to(struct ctx_s* ctx) {
  unsigned int varSP, varPC;

  // 1. Save context
  __asm("mov %0, sp" : "=r"(varSP)); // sp -> varSP
  __asm("mov %0, lr" : "=r"(varPC)); // lr -> varPC

  current_ctx->sp = varSP;
  current_ctx->pc = varPC;

  // 2. Change current context
  current_ctx = ctx;

  // 3. Restore context
  varSP = current_ctx->sp;
  varPC = current_ctx->pc;

  __asm("mov sp, %0" : : "r"(varSP)); // varSP -> sp
  __asm("mov pc, %0" : : "r"(varPC)); // varPC -> pc
}
