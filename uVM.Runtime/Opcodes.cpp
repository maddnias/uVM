#include "stdafx.h"
#include "Opcodes.h"
#include "FunctionStack.h"
#include "TypeContainer.h"

int fillFunctionParameters(FunctionContext *caller, FunctionContext *callee);
int getElementSize(DataType type);

int getElementSize(DataType type) {
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

void readOperand(char *code, unsigned int *ip, Instruction *instr) {
	// read low 32 bits
	TypeContainer container;
	container.container = (int)(*(long long*)(code + *ip) >> 32);

	instr->type = container;

	void *operand = NULL;

	//if (instr->hasSubType) {
	//	return;
	//}
	 {
		// read value from the 64 bits following initial operand
		if (getMainType(&instr->type) == DT_LONG ||
			getMainType(&instr->type) == DT_ULONG) {
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

	if (operand == NULL) {
		// ???
		return;
	}
	instr->operand = operand;
}

void op_push(Instruction *instr, FunctionContext *func) {
	switch (getMainType(&instr->type)) {
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

void op_ret(Instruction *instr, FunctionContext *func) {
	// there might not be a value on top of stack if
	// the return type is a void
	if (getMainType(&func->returnType) != DT_VOID)
		func->returnValue = stackPop(func).value;
}

void op_add(Instruction *instr, FunctionContext *func) {
	TypeContainer type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	TypeContainer type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (getMainType(&type1) == DT_STRING || getMainType(&type2) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 + val1, instr->type);
}

void op_sub(Instruction *instr, FunctionContext *func) {
	TypeContainer type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	TypeContainer type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (getMainType(&type1) == DT_STRING || getMainType(&type2) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 - val1, instr->type);
}

void op_mul(Instruction *instr, FunctionContext *func) {
	TypeContainer type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	TypeContainer type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (getMainType(&type1) == DT_STRING || getMainType(&type2) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 * val1, type1);
}

void op_div(Instruction *instr, FunctionContext *func) {
	TypeContainer type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	TypeContainer type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (getMainType(&type1) == DT_STRING || getMainType(&type2) == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 / val1, instr->type);
}

void op_lcall(Instruction *instr, FunctionContext *func, RuntimeContext *ctx) {
	unsigned short targetIdx = *(unsigned short*)instr->operand;
	if (targetIdx < 0 || targetIdx >= ctx->funcTable->tableCount)
		// out of range 
		return;

	FuncHeader *targetFuncHdr = ctx->funcTable->table[targetIdx];
	FunctionContext *targetFunc = createFunction(targetFuncHdr, ctx);

	if (fillFunctionParameters(func, targetFunc) != 0) {
		// failed to fill callee parameters
		return;
	}

	executeFunction(targetFunc, ctx);

	// there might not be a value on top of stack if
	// callee return type is void
	if (getMainType(&targetFunc->returnType) != DT_VOID) {
		stackPush(func, targetFunc->returnValue, targetFunc->returnType);
	}
	free(targetFunc);
}

int fillFunctionParameters(FunctionContext *caller, FunctionContext *callee) {
	// check if there are enough entries on stack
	if (callee->parameterCount > getStackSize(caller)) {
		return -1;
	}
	// check if parameter types match
	for (int i = 0, j = callee->parameterCount; i < callee->parameterCount; i++, j--) {
		if (getMainType(&stackPeek(caller, j).type) != getMainType(&callee->parameters[i]->type)) {
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

void op_ijmp(Instruction *instr, FunctionContext *func) {
	if (*(unsigned int*)instr->operand < 0 || *(unsigned int*)instr->operand > func->opCount)
		// invalid offset
		return;
	func->next = *(unsigned int*)instr->operand;
}

void op_alloc(Instruction *instr, FunctionContext *func, RuntimeContext *ctx) {
	int ptr = allocateMemory(*(unsigned int*)instr->operand, ctx->globalMemoryBlock);
	if (ptr == -1)
		// failed to allocate memory
		return;
	stackPush(func, ptr, instr->type);
}

void op_rarg(Instruction *instr, FunctionContext *func) {
	int argIdx = *(int*)instr->operand;
	if (argIdx < 0 || argIdx > func->parameterCount)
		// invalid parameter index
		return;

	Parameter param;
	for (int i = 0; i < func->parameterCount; i++)
		if (func->parameters[i]->index == argIdx)
			param = *func->parameters[i];

	stackPush(func, param.value, instr->type);
}

void op_mkarr(Instruction *instr, FunctionContext *func, RuntimeContext *ctx) {
	if (!hasSubType(&instr->type) || getSubType(&instr->type) != DT_ARRAY)
		// invalid type
		return;

	int elementSize = getElementSize((DataType)getMainType(&instr->type));
	int initSize = stackPop(func).value;
	int pBase = allocateMemory(initSize * elementSize, ctx->globalMemoryBlock);

	stackPush(func, pBase, instr->type);
}

void op_setelem(Instruction *instr, FunctionContext *func, RuntimeContext *ctx) {
	StackEntry value = stackPop(func);
	StackEntry elementIdx = stackPop(func);
	StackEntry pArray = stackPop(func);

	DataType elementType = (DataType)getMainType(&pArray.type);
	if ((int)elementType != getMainType(&value.type)) {
		return;
	}
	int elementSize = getElementSize(elementType);
	// copy value on stack to [pArr + (elementSize * index)] in global memory
	memcpy((char*)ctx->globalMemoryBlock->mem + (elementIdx.value * elementSize), &value.value, elementSize);

	long long testVal = 0;

	memcpy(&testVal, (char*)ctx->globalMemoryBlock->mem + (elementIdx.value * elementSize), elementSize);

	return;
}