#include "stdafx.h"
#include "FunctionStack.h"

const int MAX_STACK = 32;

struct TypeContainer *getTopStackType(struct FunctionContext *func) {
	return func->funcStack[func->stackTop - 1].type;
}

void stackPush(struct FunctionContext *func, long long value, struct TypeContainer *type) {
	if (func->stackTop + 1 > MAX_STACK)
		// stack overflow
		return;

	long long paddedValue = 0;
	switch (getMainType(type)) {
	case DT_SHORT:
	case DT_USHORT:
		memcpy(&paddedValue, &value, 2);
		break;
	case DT_INTEGER:
	case DT_UINTEGER:
	case DT_STRING:
		memcpy(&paddedValue, &value, 4);
		break;
	case DT_LONG:
	case DT_ULONG:
		memcpy(&paddedValue, &value, 8);
		break;
	case DT_CHAR:
		memcpy(&paddedValue, &value, 1);
		break;
	}

	struct StackEntry *entry = (struct StackEntry*)malloc(sizeof(struct StackEntry));
	entry->type = type;
	entry->value = paddedValue;
	func->funcStack[func->stackTop] = *entry;
	func->stackTop += 1;;
}

struct StackEntry stackPop(struct FunctionContext *func) {
	// TODO: safety checks
	func->stackTop -= 1;
	struct StackEntry entry = func->funcStack[func->stackTop];

	return entry;
}

struct StackEntry *stackPeekTop(struct FunctionContext *func) {
	struct StackEntry *entry = &func->funcStack[func->stackTop - 1];
	return entry;
}

struct StackEntry *stackPeek(struct FunctionContext *func, int depth) {
	struct StackEntry *entry = &func->funcStack[func->stackTop - depth];
	return entry;
}

int getStackSize(struct FunctionContext *func) {
	return func->stackTop ;
}

int verifyStack(struct FunctionContext *func, enum StackBehaviour behaviour) {
	switch (behaviour)
	{
	case Push1:
		if (func->stackTop + 1 > MAX_STACK)
			return -1;
	case Push2:
		if (func->stackTop + 2 > MAX_STACK)
			return -1;
		break;
	case Pop1:
		if (func->stackTop - 1 < 0)
			return -1;
		break;
	case Pop2:
		if (func->stackTop - 2 < 0)
			return -1;
		break;
	}
	return 0;
}