#include "stdafx.h"
#include "Executor.h"
#include "MemoryAllocator.h"

void readOperand(char *code, unsigned int *ip, struct Instruction *instr);
void op_push(struct Instruction *instr, struct FunctionContext *func);
void op_ret(struct Instruction *instr, struct FunctionContext *func);
void op_add(struct Instruction *instr, struct FunctionContext *func);
void op_sub(struct Instruction *instr, struct FunctionContext *func);
void op_mul(struct Instruction *instr, struct FunctionContext *func);
void op_div(struct Instruction *instr, struct FunctionContext *func);
void op_lcall(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx);
void op_ijmp(struct Instruction *instr, struct FunctionContext *func);
void op_alloc(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx);
void op_rarg(struct Instruction *instr, struct FunctionContext *func);
void op_mkarr(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx);
void op_setelem(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx);
void op_getelem(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx);
void op_setvar(struct Instruction *instr, struct FunctionContext *func);
void op_getvar(struct Instruction *instr, struct FunctionContext *func);
void op_jle(struct Instruction *instr, struct FunctionContext *func);