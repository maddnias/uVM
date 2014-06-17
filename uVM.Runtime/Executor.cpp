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
	case Push:
		op_push(instr, func);
		break;
	case Add:
		op_add(instr, func);
		break;
	case Sub:
		op_sub(instr, func);
		break;
	case Mul:
		op_mul(instr, func);
		break;
	case Div:
		op_div(instr, func);
		break;
	case LCall:
		op_lcall(instr, func, ctx);
		break;
	case IJmp:
		op_ijmp(instr, func);
		break;
	case RARG:
		op_rarg(instr, func);
		break;
	case Ret:
		op_ret(instr, func);
		return 0;
	}

	if (instr->opcode != IJmp) {
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
	case Push:
		instr->hasOperand = true;
		instr->opcode = Push;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case Add:
		instr->hasOperand = false;
		instr->opcode = Add;
		instr->stackBehaviour = Push1;
		break;
	case Sub:
		instr->hasOperand = false;
		instr->opcode = Sub;
		instr->stackBehaviour = Push1;
		break;
	case Mul:
		instr->hasOperand = false;
		instr->opcode = Mul;
		instr->stackBehaviour = Push1;
		break;
	case Div:
		instr->hasOperand = false;
		instr->opcode = Div;
		instr->stackBehaviour = Push1;
		break;
	case Ret:
		instr->hasOperand = false;
		instr->opcode = Ret;
		instr->stackBehaviour = None;
		break;
	case LCall:
		instr->hasOperand = true;
		instr->opcode = LCall;
		instr->stackBehaviour = Push1;
		readOperand(code, ip, instr);
		break;
	case IJmp:
		instr->hasOperand = true;
		instr->opcode = IJmp;
		instr->stackBehaviour = None;
		readOperand(code, ip, instr);
		break;
	case RARG:
		instr->hasOperand = true;
		instr->opcode = RARG;
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
	func->returnType = (DataType)hdr->returnType;
	func->parameterCount = hdr->paramCount;

	if (func->parameterCount > 0) {
		func->parameters = (Parameter**)malloc(sizeof(Parameter*));
		for (int i = 0; i < hdr->paramCount; i++) {
			func->parameters[i] = (Parameter*)malloc(sizeof(Parameter));
			func->parameters[i]->index = i;
			func->parameters[i]->type = (DataType)*(char*)hdr->paramTypes[i];
		}
	}

	return func;
}