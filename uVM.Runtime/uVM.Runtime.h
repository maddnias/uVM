#include "stdafx.h"
#include <stdlib.h>


#pragma once

struct ParamHeader {
	int index;
	int typeContainer;
};

struct ParamTable {
	unsigned int tableCount;
	struct ParamHeader **table;
};

struct VariableHeader {
	int index;
	int typeContainer;
};

struct VariableTable {
	unsigned int tableCount;
	struct VariableHeader **table;
};

struct FuncHeader {
	char returnType;
	unsigned int paramCount;
	struct ParamTable *paramTable;
	unsigned int variableCount;
	struct VariableTable *variableTable;
	int opCount;
	int codeSize;
	char *code;
};

struct FuncTable {
	unsigned int tableCount;
	struct FuncHeader **table;
};


struct MemorySlot {
	int state;
	int size;
};

struct MemoryBlock {
	void *mem;
	struct MemorySlot memorySlots[512];
};

struct RuntimeContext {
	struct FuncTable *funcTable;
	unsigned int *ip;
	struct MemoryBlock *globalMemoryBlock;
	char *file;
	char *code;
	BOOL isEOF;
};

struct RuntimeContext *createRuntimeContext(char *file);