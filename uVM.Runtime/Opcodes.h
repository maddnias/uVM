#include "stdafx.h"
#include "Executor.h"
#include "MemoryAllocator.h"

void readOperand(char *code, unsigned int *ip, Instruction *instr);
void op_push(Instruction *instr, FunctionContext *func);
void op_ret(Instruction *instr, FunctionContext *func);
void op_add(Instruction *instr, FunctionContext *func);
void op_sub(Instruction *instr, FunctionContext *func);
void op_mul(Instruction *instr, FunctionContext *func);
void op_div(Instruction *instr, FunctionContext *func);
void op_lcall(Instruction *instr, FunctionContext *func, RuntimeContext *ctx);
void op_ijmp(Instruction *instr, FunctionContext *func);
void op_alloc(Instruction *instr, FunctionContext *func, RuntimeContext *ctx);
void op_rarg(Instruction *instr, FunctionContext *func);
void op_mkarr(Instruction *instr, FunctionContext *func, RuntimeContext *ctx);
void op_setelem(Instruction *instr, FunctionContext *func, RuntimeContext *ctx);
void op_getelem(Instruction *instr, FunctionContext *func, RuntimeContext *ctx);
void op_setvar(Instruction *instr, FunctionContext *func);
void op_getvar(Instruction *instr, FunctionContext *func);