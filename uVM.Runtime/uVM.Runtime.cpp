// uVM.Runtime.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "uVM.Runtime.h"
#include "MemoryAllocator.h"

RuntimeContext *createRuntimeContext(char *file) {
	RuntimeContext *ctx = (RuntimeContext*)malloc(sizeof(RuntimeContext));

	ctx->ip = (unsigned int*)malloc(sizeof(unsigned int));
	ctx->globalMemoryBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	ctx->globalMemoryBlock->mem = malloc(4096);

	for (int i = 0; i < (4096 / MSLOT_SIZE); i++)
		ctx->globalMemoryBlock->memorySlots[i].state = MSLOT_FREE;

	ctx->isEOF = false;
	ctx->file = file;
	return ctx;
}