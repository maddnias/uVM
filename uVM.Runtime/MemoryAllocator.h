#include "stdafx.h"
#include "uVM.Runtime.h"

int allocateMemory(int size, MemoryBlock *mem);
int deallocateMemory(int ptr, MemoryBlock *mem);

#pragma once
const int MSLOT_SIZE = 8;

enum MemorySlotType {
	MSLOT_FREE = 0,
	MSLOT_USED = 1,
	MSLOT_POINTERBASE = 2
};