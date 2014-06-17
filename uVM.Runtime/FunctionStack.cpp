#include "stdafx.h"
#include "FunctionStack.h"

DataType getTopStackType(FunctionContext *func) {
	return func->funcStack[func->stackTop -1].type;
}

void stackPush(FunctionContext *func, long long value, DataType type) {
	if (func->stackTop + 1 > MAX_STACK)
		// stack overflow
		return;

	long long paddedValue = 0;
	switch (type) {
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

	StackEntry *entry = (StackEntry*)malloc(sizeof(StackEntry));
	entry->type = type;
	entry->value = paddedValue;
	func->funcStack[func->stackTop] = *entry;
	func->stackTop += 1;;
}

StackEntry stackPop(FunctionContext *func) {
	// TODO: safety checks
	func->stackTop -= 1;
	StackEntry entry = func->funcStack[func->stackTop];

	return entry;
}

StackEntry stackPeek(FunctionContext *func) {
	StackEntry entry = func->funcStack[func->stackTop - 1];
	return entry;
}

StackEntry stackPeek(FunctionContext *func, int depth) {
	StackEntry entry = func->funcStack[func->stackTop - depth];
	return entry;
}

int getStackSize(FunctionContext *func) {
	return func->stackTop ;
}