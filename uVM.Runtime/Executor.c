#include "stdafx.h"
#include "uVM.Runtime.h"
#include "Executor.h"
#include "Opcodes.h"

int readInstruction(char *code, unsigned int *ip, struct Instruction *instr);
int executeInstruction(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx);

void executeFunction(struct FunctionContext *func, struct RuntimeContext *ctx) {
	struct Instruction **opcodes = (struct Instruction**)malloc(sizeof(struct Instruction*) * func->opCount);

	for (int i = 0; i <= func->opCount; i++) {
		opcodes[i] = (struct Instruction*)malloc(sizeof(struct Instruction));
	}

	for (int i = 0; i < func->opCount; i++) {
		if (!readInstruction(func->code, &func->localIp, opcodes[i]))
			break;
	}

	func->next = 0;
	while (executeInstruction(opcodes[func->next], func, ctx)) {
	}
}

int executeInstruction(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx) {
	switch (instr->opcode) {
	case PUSH:
		op_push(instr, func);
		break;
	case ADD:
		op_add(instr, func);
		break;
	case SUB:
		op_sub(instr, func);
		break;
	case MUL:
		op_mul(instr, func);
		break;
	case DIV:
		op_div(instr, func);
		break;
	case LCALL:
		op_lcall(instr, func, ctx);
		break;
	case IJMP:
		op_ijmp(instr, func);
		break;
	case RARG:
		op_rarg(instr, func);
		break;
	case MKARR:
		op_mkarr(instr, func, ctx);
		break;
	case SETELEM:
		op_setelem(instr, func, ctx);
		break;
	case GETELEM:
		op_getelem(instr, func, ctx);
		break;
	case SETVAR:
		op_setvar(instr, func);
		break;
	case GETVAR:
		op_getvar(instr, func);
		break;
	case JLE:
		op_jle(instr, func);
		break;
	case RET:
		op_ret(instr, func);
		return 0;
	}

	if (instr->opcode != IJMP) {
		if (func->next +1 <= func->opCount)
			func->next++;
	}

	return 1;
}

int readInstruction(char *code, unsigned int *ip, struct Instruction *instr) {
	enum OpCode opcode = (enum OpCode)code[*ip];
	*ip += sizeof(enum OpCode);
	
	instr->offset = *ip;

	switch (opcode) {
	case PUSH:
		instr->hasOperand = TRUE;
		instr->opcode = PUSH;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case ADD:
		instr->hasOperand = FALSE;
		instr->opcode = ADD;
		instr->stackBehaviour = Push1;
		break;
	case SUB:
		instr->hasOperand = FALSE;
		instr->opcode = SUB;
		instr->stackBehaviour = Push1;
		break;
	case MUL:
		instr->hasOperand = FALSE;
		instr->opcode = MUL;
		instr->stackBehaviour = Push1;
		break;
	case DIV:
		instr->hasOperand = FALSE;
		instr->opcode = DIV;
		instr->stackBehaviour = Push1;
		break;
	case RET:
		instr->hasOperand = FALSE;
		instr->opcode = RET;
		instr->stackBehaviour = None;
		break;
	case LCALL:
		instr->hasOperand = TRUE;
		instr->opcode = LCALL;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case IJMP:
		instr->hasOperand = TRUE;
		instr->opcode = IJMP;
		instr->stackBehaviour = None;
		readOperand(code, ip, instr);
		break;
	case RARG:
		instr->hasOperand = TRUE;
		instr->opcode = RARG;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case MKARR: 
		instr->hasOperand = TRUE;
		instr->opcode = MKARR;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case SETELEM:
		instr->hasOperand = TRUE;
		instr->opcode = SETELEM;
		instr->stackBehaviour = Pop2;
		readOperand(code, ip, instr);
		break;
	case GETELEM:
		instr->hasOperand = TRUE;
		instr->opcode = GETELEM;
		instr->stackBehaviour = Pop1;
		break;
	case SETVAR:
		instr->hasOperand = TRUE;
		instr->opcode = SETVAR;
		instr->stackBehaviour = Pop1;
		readOperand(code, ip, instr);
		break;
	case GETVAR:
		instr->hasOperand = TRUE;
		instr->opcode = GETVAR;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case JLE:
		instr->hasOperand = TRUE;
		instr->opcode = JLE;
		instr->stackBehaviour = None;
		readOperand(code, ip, instr);
		break;
	default:
		return 0;
	}

	return 1;
}

struct FunctionContext *createFunction(struct FuncHeader *hdr, struct RuntimeContext *ctx) {
	struct FunctionContext *func = (struct FunctionContext*)malloc(sizeof(struct FunctionContext));
	func->codeSize = hdr->codeSize;
	func->code = hdr->code;
	func->localIp = 0;
	func->stackTop = 0;
	func->opCount = hdr->opCount;
	struct TypeContainer *container = (struct TypeContainer*)malloc(sizeof(struct TypeContainer));
	container->container = (int)hdr->returnType;
	func->returnType = container;
	func->parameterCount = hdr->paramCount;
	func->variableCount = hdr->variableCount;
	func->next = 0;

	func->parameters[0] = (struct Parameter*)malloc(sizeof(struct Parameter*) * func->parameterCount);
	func->variables[0] = (struct Variable*)malloc(sizeof(struct Variable*) * func->variableCount);

	if (func->parameterCount > 0) {
		for (int i = 0; i < hdr->paramCount; i++) {
			func->parameters[i]->index = i;
			container->container = hdr->paramTable->table[i]->typeContainer;
			func->parameters[i]->type = container;
		}
	}

	if (func->variableCount > 0) {
		for (int i = 0; i < hdr->variableCount; i++) {
			func->variables[i]->index = i;
			container->container = hdr->variableTable->table[i]->typeContainer;
			func->variables[i]->type = container;
		}
	}
	return func;
}