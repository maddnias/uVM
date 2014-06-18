#include "stdafx.h"
#include "uVM.Runtime.h"
#include "Executor.h"
#include "Opcodes.h"

int readInstruction(char *code, unsigned int *ip, Instruction *instr);
int executeInstruction(Instruction *instr, FunctionContext *func, RuntimeContext *ctx);

void executeFunction(FunctionContext *func, RuntimeContext *ctx) {
	Instruction **opcodes = (Instruction**)malloc(sizeof(Instruction**));

	for (int i = 0; i <= func->opCount; i++) {
		opcodes[i] = (Instruction*)malloc(sizeof(Instruction));
	}

	for (int i = 0; i < func->opCount; i++) {
		if (!readInstruction(func->code, &func->localIp, opcodes[i]))
			break;
	}

	func->next = 0;
	while (executeInstruction(opcodes[func->next], func, ctx)) {
	}
}

int executeInstruction(Instruction *instr, FunctionContext *func, RuntimeContext *ctx) {
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

int readInstruction(char *code, unsigned int *ip, Instruction *instr) {
	OpCode opcode = (OpCode)code[*ip];
	*ip += sizeof(OpCode);
	
	instr->offset = *ip;

	switch (opcode) {
	case PUSH:
		instr->hasOperand = true;
		instr->opcode = PUSH;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case ADD:
		instr->hasOperand = false;
		instr->opcode = ADD;
		instr->stackBehaviour = Push1;
		break;
	case SUB:
		instr->hasOperand = false;
		instr->opcode = SUB;
		instr->stackBehaviour = Push1;
		break;
	case MUL:
		instr->hasOperand = false;
		instr->opcode = MUL;
		instr->stackBehaviour = Push1;
		break;
	case DIV:
		instr->hasOperand = false;
		instr->opcode = DIV;
		instr->stackBehaviour = Push1;
		break;
	case RET:
		instr->hasOperand = false;
		instr->opcode = RET;
		instr->stackBehaviour = None;
		break;
	case LCALL:
		instr->hasOperand = true;
		instr->opcode = LCALL;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case IJMP:
		instr->hasOperand = true;
		instr->opcode = IJMP;
		instr->stackBehaviour = None;
		readOperand(code, ip, instr);
		break;
	case RARG:
		instr->hasOperand = true;
		instr->opcode = RARG;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case MKARR: 
		instr->hasOperand = true;
		instr->opcode = MKARR;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case SETELEM:
		instr->hasOperand = true;
		instr->opcode = SETELEM;
		instr->stackBehaviour = Pop2;
		readOperand(code, ip, instr);
		break;
	case GETELEM:
		instr->hasOperand = true;
		instr->opcode = GETELEM;
		instr->stackBehaviour = Pop1;
		break;
	case SETVAR:
		instr->hasOperand = true;
		instr->opcode = SETVAR;
		instr->stackBehaviour = Pop1;
		readOperand(code, ip, instr);
		break;
	case GETVAR:
		instr->hasOperand = true;
		instr->opcode = GETVAR;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	default:
		return 0;
	}

	return 1;
}

FunctionContext *createFunction(FuncHeader *hdr, RuntimeContext *ctx) {
	FunctionContext *func = (FunctionContext*)malloc(sizeof(FunctionContext));
	func->codeSize = hdr->codeSize;
	func->code = hdr->code;
	func->localIp = 0;
	func->stackTop = 0;
	func->opCount = hdr->opCount;
	TypeContainer container;
	container.container = (int)hdr->returnType;
	func->returnType = container;
	func->parameterCount = hdr->paramCount;
	func->variableCount = hdr->variableCount;

	if (func->parameterCount > 0) {
		func->parameters = (Parameter**)malloc(sizeof(Parameter*));
		for (int i = 0; i < hdr->paramCount; i++) {
			func->parameters[i] = (Parameter*)malloc(sizeof(Parameter));
			func->parameters[i]->index = i;
			container.container = hdr->paramTable->table[i]->typeContainer;
			func->parameters[i]->type = container;
		}
	}

	if (func->variableCount > 0) {
		func->variables = (Variable**)malloc(sizeof(Variable*));
		for (int i = 0; i < hdr->variableCount; i++) {
			func->variables[i] = (Variable*)malloc(sizeof(Variable));
			func->variables[i]->index = i;
			container.container = hdr->variableTable->table[i]->typeContainer;
			func->variables[i]->type = container;
		}
	}
	return func;
}