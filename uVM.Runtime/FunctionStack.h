#include "stdafx.h"
#include "uVM.Runtime.h"
#include "Executor.h"

void stackPush(struct FunctionContext *func, long long value, struct TypeContainer *type);
struct StackEntry stackPop(struct FunctionContext *func);
struct TypeContainer *getTopStackType(struct FunctionContext *func);
struct StackEntry *stackPeek(struct FunctionContext *func);
struct StackEntry *stackPeek(struct FunctionContext *func, int depth);
int getStackSize(struct FunctionContext *func);
int verifyStack(struct FunctionContext *func, enum StackBehaviour behaviour);