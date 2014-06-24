#include "stdafx.h"
#include "uVM.Runtime.h"
#include "TypeContainer.h"

extern const int MAX_STACK;

#pragma once


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
	GETELEM,
	SETVAR,
	GETVAR,
	JLE,
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
	enum OpCode opcode;
	BOOL hasOperand;
	int operandSize;
	enum StackBehaviour stackBehaviour;
	void *operand;
	struct TypeContainer *type;
};

struct StackEntry {
	long long value;
	struct TypeContainer *type;
};

struct Parameter {
	int index;
	long long value;
	struct TypeContainer *type;
};

struct Variable {
	int index;
	long long value;
	struct TypeContainer *type;
};

struct FunctionContext {
	int next;
	int opCount;
	char stackTop;
	struct StackEntry funcStack[32];
	int parameterCount;
	int variableCount;
	int codeSize;
	char *code;
	unsigned int localIp;
	long returnValue;
	struct TypeContainer *returnType;
	struct Parameter *parameters[1];
	struct Variable *variables[1];
};

void executeFunction(struct FunctionContext *func, struct RuntimeContext *ctx);
struct FunctionContext *createFunction(struct FuncHeader *hdr, struct RuntimeContext *ctx);