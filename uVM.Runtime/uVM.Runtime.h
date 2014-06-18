#include "stdafx.h"
#include <stdlib.h>


#pragma once

struct FuncHeader {
	char returnType;
	unsigned int paramCount;
	int **paramTypes;
	int opCount;
	int codeSize;
	char *code;
};

struct FuncTable {
	unsigned int tableCount;
	FuncHeader **table;
};

struct MemorySlot {
	int state;
	int size;
};

struct MemoryBlock {
	void *mem;
	MemorySlot memorySlots[512];
};

struct RuntimeContext {
	FuncTable *funcTable;
	unsigned int *ip;
	MemoryBlock *globalMemoryBlock;
	char *file;
	char *code;
	BOOL isEOF;
};

RuntimeContext *createRuntimeContext(char *file);