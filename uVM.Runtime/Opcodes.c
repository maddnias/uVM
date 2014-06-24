#include "stdafx.h"
#include "Opcodes.h"
#include "FunctionStack.h"
#include "TypeContainer.h"

int fillFunctionParameters(struct FunctionContext *caller, struct FunctionContext *callee);
int getElementSize(enum DataType type);

int getElementSize(enum DataType type) {
	switch (type)
	{
	case DT_SHORT:
	case DT_USHORT:
		return 2;
		break;
	case DT_INTEGER:
	case DT_UINTEGER:
	case DT_STRING:
		return 4;
		break;
	case DT_LONG:
	case DT_ULONG:
		return 8;
		break;
	default:
		return -1;
	}
}

void readOperand(char *code, unsigned int *ip, struct Instruction *instr) {
	// read low 32 bits
	struct TypeContainer *container = (struct TypeContainer*)malloc(sizeof(struct TypeContainer));
	container->container = (int)(*(long long*)(code + *ip) >> 32);
	instr->type = container;

	void *operand = NULL;

	//if (instr->hasSubType) {
	//	return;
	//}
	 {
		// read value from the 64 bits following initial operand
		if (getMainType(instr->type) == DT_LONG ||
			getMainType(instr->type) == DT_ULONG) {
			operand = (long long*)malloc(sizeof(long long));
			*(long long*)operand = *(long long*)(code + *ip + 8);
			*ip += 16;
		}
		// read value from the lower 32 bits of the operand
		else {
			operand = (int*)malloc(sizeof(int));
			*(int*)operand = (int)(*(long long*)(code + *ip) & MAXUINT);
			*ip += 8;
		}
	}

	instr->operand = operand;
}

void op_push(struct Instruction *instr, struct FunctionContext *func) {
	switch (getMainType(instr->type)) {
	case DT_SHORT:
		stackPush(func, *(short*)instr->operand, instr->type);
		break;
	case DT_INTEGER:
		stackPush(func, *(int*)instr->operand, instr->type);
		break;
	case DT_LONG:
		stackPush(func, *(long long*)instr->operand, instr->type);
		break;
	case DT_USHORT:
		stackPush(func, *(unsigned short*)instr->operand, instr->type);
		break;
	case DT_UINTEGER:
		stackPush(func, *(unsigned int*)instr->operand, instr->type);
		break;
	case DT_ULONG:
		stackPush(func, *(unsigned long long*)instr->operand, instr->type);
		break;
	}
}

void op_ret(struct Instruction *instr, struct FunctionContext *func) {
	// there might not be a value on top of stack if
	// the return type is a void
	if (getMainType(func->returnType) != DT_VOID)
		func->returnValue = stackPop(func).value;
}

void op_add(struct Instruction *instr, struct FunctionContext *func) {
	struct StackEntry val1 = stackPop(func);
	struct StackEntry val2 = stackPop(func);

	if (getMainType(val1.type) == DT_STRING || getMainType(val2.type) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// TODO: more type checks on values
	stackPush(func, val1.value + val2.value, val1.type);
}

void op_sub(struct Instruction *instr, struct FunctionContext *func) {
	struct StackEntry val1 = stackPop(func);
	struct StackEntry val2 = stackPop(func);

	if (getMainType(val1.type) == DT_STRING || getMainType(val2.type) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// TODO: more type checks on values
	stackPush(func, val1.value - val2.value, val1.type);
}

void op_mul(struct Instruction *instr, struct FunctionContext *func) {
	struct StackEntry val1 = stackPop(func);
	struct StackEntry val2 = stackPop(func);

	if (getMainType(val1.type) == DT_STRING || getMainType(val2.type) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// TODO: more type checks on values
	stackPush(func, val1.value * val2.value, val1.type);
}

void op_div(struct Instruction *instr, struct FunctionContext *func) {
	struct StackEntry val1 = stackPop(func);
	struct StackEntry val2 = stackPop(func);

	if (getMainType(val1.type) == DT_STRING || getMainType(val2.type) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// TODO: more type checks on values
	stackPush(func, val1.value / val2.value, val1.type);
}

void op_lcall(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx) {
	unsigned short targetIdx = *(unsigned short*)instr->operand;
	if (targetIdx < 0 || targetIdx >= ctx->funcTable->tableCount)
		// out of range 
		return;

	struct FuncHeader *targetFuncHdr = ctx->funcTable->table[targetIdx];
	struct FunctionContext *targetFunc = createFunction(targetFuncHdr, ctx);

	if (fillFunctionParameters(func, targetFunc) != 0) {
		// failed to fill callee parameters
		return;
	}

	executeFunction(targetFunc, ctx);

	// there might not be a value on top of stack if
	// callee return type is void
	if (getMainType(targetFunc->returnType) != DT_VOID) {
		stackPush(func, targetFunc->returnValue, targetFunc->returnType);
	}
	free(targetFunc);
}

int fillFunctionParameters(struct FunctionContext *caller, struct FunctionContext *callee) {
	// check if there are enough entries on stack
	if (callee->parameterCount > getStackSize(caller)) {
		return -1;
	}
	// check if parameter types match
	for (int i = 0, j = callee->parameterCount; i < callee->parameterCount; i++, j--) {
		if (getMainType(stackPeek(caller, j)->type) != getMainType(callee->parameters[i]->type)) {
			return -1;
		}
	}
	// fill callee parameters
	for (int i = callee->parameterCount; i > 0; i--) {
		callee->parameters[i -1]->value = stackPop(caller).value;
	}
	// ready to call
	return 0;
}

void op_ijmp(struct Instruction *instr, struct FunctionContext *func) {
	if (*(unsigned int*)instr->operand < 0 || *(unsigned int*)instr->operand > func->opCount)
		// invalid offset
		return;
	func->next = *(unsigned int*)instr->operand;
}

void op_alloc(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx) {
	int ptr = allocateMemory(*(unsigned int*)instr->operand, ctx->globalMemoryBlock);
#ifdef _DEBUG
	assert(ptr >= 0 && ptr <= 4096);
#endif
	stackPush(func, ptr, instr->type);
}

void op_rarg(struct Instruction *instr, struct FunctionContext *func) {
	int argIdx = *(int*)instr->operand;
	if (argIdx < 0 || argIdx > func->parameterCount)
		// invalid parameter index
		return;

	struct Parameter param;
	for (int i = 0; i < func->parameterCount; i++)
		if (func->parameters[i]->index == argIdx)
			param = *func->parameters[i];

	stackPush(func, param.value, instr->type);
}

void op_mkarr(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx) {
	if (!hasSubType(instr->type) || getSubType(instr->type) != DT_ARRAY)
		// invalid type
		return;

	int elementSize = getElementSize((enum DataType)getMainType(instr->type));
	int initSize = stackPop(func).value;
	int pBase = allocateMemory(initSize * elementSize, ctx->globalMemoryBlock);

#ifdef _DEBUG
	assert(pBase >= 0 && pBase <= 4096);
#endif
	stackPush(func, pBase, instr->type);
}

void op_setelem(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx) {
	struct StackEntry value = stackPop(func);
	struct StackEntry elementIdx = stackPop(func);
	struct StackEntry pArray = stackPop(func);

	enum DataType elementType = (enum DataType)getMainType(pArray.type);
	if ((int)elementType != getMainType(value.type)) {
		return;
	}
	int elementSize = getElementSize(elementType);
#ifdef _DEBUG
	assert(elementSize != -1);
#endif
	// copy value on stack to [pArr + (elementSize * index)] in global memory
	memcpy((char*)ctx->globalMemoryBlock->mem + (elementIdx.value * elementSize), &value.value, elementSize);

#ifdef _DEBUG
	long long testVal = 0;
	memcpy(&testVal, (char*)ctx->globalMemoryBlock->mem + pArray.value + (elementIdx.value * elementSize), elementSize);
	assert(testVal == value.value);
	assert((elementSize * elementIdx.value) < getPaddedPointerDataSize(pArray.value, ctx->globalMemoryBlock));
#endif
}

void op_getelem(struct Instruction *instr, struct FunctionContext *func, struct RuntimeContext *ctx) {
	struct StackEntry elementIdx = stackPop(func);
	struct StackEntry pArray = stackPop(func);

	enum DataType elementType = (enum DataType)getMainType(pArray.type);
	int elementSize = getElementSize(elementType);

	long long value = 0;
	memcpy(&value, (char*)ctx->globalMemoryBlock->mem + pArray.value + (elementIdx.value * elementSize), elementSize);
	stackPush(func, value, pArray.type);
}

void op_setvar(struct Instruction *instr, struct FunctionContext *func) {
	int varIdx = *(int*)instr->operand;
	if (varIdx < 0 || varIdx > func->variableCount)
		// invalid index
		return;

	// more checks?

	struct StackEntry value = stackPop(func);
	func->variables[varIdx]->type = value.type;
	func->variables[varIdx]->value = value.value;
}

void op_getvar(struct Instruction *instr, struct FunctionContext *func) {
	int varIdx = *(int*)instr->operand;
	if (varIdx < 0 || varIdx > func->variableCount)
		// invalid index
		return;

	// more checks?

	stackPush(func, func->variables[varIdx]->value, func->variables[varIdx]->type);
}

void op_jle(struct Instruction *instr, struct FunctionContext *func) {
	if (*(unsigned int*)instr->operand < 0 || *(unsigned int*)instr->operand > func->opCount)
		// invalid offset
		return;
	struct StackEntry val2 = stackPop(func);
	struct StackEntry val1 = stackPop(func);

	if (val1.value <= val2.value) {
		func->next = *(unsigned int*)instr->operand;
	}
}