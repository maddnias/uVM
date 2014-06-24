#include "stdafx.h"
#include "uVM.Runtime.h"

int allocateMemory(int size, struct MemoryBlock *mem);
int deallocateMemory(int ptr, struct MemoryBlock *mem);
int getPaddedPointerDataSize(int ptr, struct MemoryBlock *mem);
extern const int MSLOT_SIZE;

#pragma once


enum MemorySlotType {
	MSLOT_FREE = 0,
	MSLOT_USED = 1,
	MSLOT_POINTERBASE = 2
};