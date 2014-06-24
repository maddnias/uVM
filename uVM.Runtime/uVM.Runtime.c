// uVM.Runtime.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "uVM.Runtime.h"
#include "MemoryAllocator.h"

struct RuntimeContext *createRuntimeContext(char *file) {
	struct RuntimeContext *ctx = (struct RuntimeContext*)malloc(sizeof(struct RuntimeContext));

	ctx->ip = (unsigned int*)malloc(sizeof(unsigned int));
	ctx->globalMemoryBlock = (struct MemoryBlock*)malloc(sizeof(struct MemoryBlock));
	ctx->globalMemoryBlock->mem = malloc(4096);

	for (int i = 0; i < (4096 / MSLOT_SIZE); i++)
		ctx->globalMemoryBlock->memorySlots[i].state = MSLOT_FREE;

	ctx->isEOF = FALSE;
	ctx->file = file;
	return ctx;
}