#include "stdafx.h"
#include "uVM.Runtime.h"
#include "TypeContainer.h"

#pragma once
const int MAX_STACK = 32;

struct StackFrame {
	unsigned int stackCount;
	void *stack;
	unsigned int *ip;
};

enum StackBehaviour {
	Push1 = 0,
	Push2 = 1,
	Pop1 = 2,
	Pop2 = 4,
	None = 8
};

enum OpCode {
	PUSH,
	RET,
	ADD,
	LCALL,
	SUB,
	MUL,
	DIV,
	IJMP,
	ALLOC,
	RARG,
	RLOC,
	MKARR,
	SETELEM,
};

enum DataType {
	DT_SHORT,
	DT_INTEGER,
	DT_LONG,
	DT_STRING,
	DT_CHAR,
	DT_USHORT,
	DT_UINTEGER,
	DT_ULONG,
	DT_VOID,
	DT_ARRAY
};

struct Instruction {
	unsigned int offset;
	OpCode opcode;
	BOOL hasOperand;
	int operandSize;
	StackBehaviour stackBehaviour;
	void *operand;
	TypeContainer type;
};

struct StackEntry {
	long long value;
	TypeContainer type;
};

struct Parameter {
	int index;
	long long value;
	TypeContainer type;
};

struct FunctionContext {
	int next;
	int opCount;
	char stackTop;
	StackEntry funcStack[MAX_STACK];
	StackFrame **frames;
	Parameter **parameters;
	int parameterCount;
	int codeSize;
	char *code;
	unsigned int localIp;
	long returnValue;
	TypeContainer returnType;
};

void executeFunction(FunctionContext *func, RuntimeContext *ctx);
FunctionContext *createFunction(FuncHeader *hdr, RuntimeContext *ctx);