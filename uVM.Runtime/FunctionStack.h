#include "stdafx.h"
#include "uVM.Runtime.h"
#include "Executor.h"

void stackPush(FunctionContext *func, long long value, DataType type);
StackEntry stackPop(FunctionContext *func);
DataType getTopStackType(FunctionContext *func);
StackEntry stackPeek(FunctionContext *func);
StackEntry stackPeek(FunctionContext *func, int depth);
int getStackSize(FunctionContext *func);