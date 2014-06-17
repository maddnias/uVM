#include "stdafx.h"
#include "Opcodes.h"
#include "FunctionStack.h"
#include "TypeContainer.h"

int fillFunctionParameters(FunctionContext *caller, FunctionContext *callee);

void readOperand(char *code, unsigned int *ip, Instruction *instr) {
	// read low 32 bits
	TypeContainer container;
	container.container = (int)(*(long long*)(code + *ip) >> 32);

	instr->operandMainType = (DataType)getMainType(&container);
	instr->operandSubType = (DataType)getSubType(&container);
	instr->hasSubType = hasSubType(&container);

	void *operand = NULL;

	if (hasSubType(&container)) {
		// TODO: implement array and pointer types
	}
	else {
		// read value from the 64 bits following initial operand
		if (instr->operandMainType == DT_LONG ||
			instr->operandMainType == DT_ULONG) {
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
	switch (instr->operandMainType) {
	case DT_SHORT:
		stackPush(func, *(short*)instr->operand, instr->operandMainType);
		break;
	case DT_INTEGER:
		stackPush(func, *(int*)instr->operand, instr->operandMainType);
		break;
	case DT_LONG:
		stackPush(func, *(long long*)instr->operand, instr->operandMainType);
		break;
	case DT_USHORT:
		stackPush(func, *(unsigned short*)instr->operand, instr->operandMainType);
		break;
	case DT_UINTEGER:
		stackPush(func, *(unsigned int*)instr->operand, instr->operandMainType);
		break;
	case DT_ULONG:
		stackPush(func, *(unsigned long long*)instr->operand, instr->operandMainType);
		break;
	}
}

void op_ret(Instruction *instr, FunctionContext *func) {
	// there might not be a value on top of stack if
	// the return type is a void
	if (func->returnType != DT_VOID)
		func->returnValue = stackPop(func).value;
}

void op_add(Instruction *instr, FunctionContext *func) {
	DataType type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	DataType type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (type1 == DT_STRING || type2 == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 + val1, instr->operandMainType);
}

void op_sub(Instruction *instr, FunctionContext *func) {
	DataType type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	DataType type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (type1 == DT_STRING || type2 == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 - val1, instr->operandMainType);
}

void op_mul(Instruction *instr, FunctionContext *func) {
	DataType type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	DataType type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (type1 == DT_STRING || type2 == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 * val1, type1);
}

void op_div(Instruction *instr, FunctionContext *func) {
	DataType type1 = getTopStackType(func);
	long long val1 = stackPop(func).value;
	DataType type2 = getTopStackType(func);
	long long val2 = stackPop(func).value;

	if (type1 == DT_STRING || type2 == DT_STRING)
		// arithmetic on strings? think not
		return;
	// swap places of val1 and val2 because of stack structure
	stackPush(func, val2 / val1, instr->operandMainType);
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
	if (targetFunc->returnType != DT_VOID) {
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
		if (stackPeek(caller, j).type != callee->parameters[i]->type) {
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
	stackPush(func, ptr, DT_INTEGER);
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

	stackPush(func, param.value, instr->operandMainType);
}