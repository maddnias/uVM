#include "stdafx.h"
#include "uVM.Runtime.h"
#include "Executor.h"

void stackPush(FunctionContext *func, long long value, TypeContainer type);
StackEntry stackPop(FunctionContext *func);
TypeContainer getTopStackType(FunctionContext *func);
StackEntry stackPeek(FunctionContext *func);
StackEntry stackPeek(FunctionContext *func, int depth);
int getStackSize(FunctionContext *func);