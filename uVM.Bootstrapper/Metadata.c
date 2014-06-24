#include "stdafx.h"
#include "Metadata.h"
#include "ErrorCodes.h"

struct FuncTable *readFuncTable(char *fileBuff, unsigned int *pos);
struct ParamTable *readParamTable(char *fileBuff, unsigned int *pos);
struct VariableTable *readVariableTable(char *fileBuff, unsigned int *pos);

int finalizeRuntimeContext(struct RuntimeContext *ctx) {
	readMetadata(ctx, ctx->file);

	return 0;
}

int readMetadata(struct RuntimeContext *ctx, char *fileBuff) {
	unsigned int i = 0;

	struct FileHeader *fileHdr = (struct FileHeader*)(fileBuff);
	// advance over file header
	i += sizeof(struct FileHeader);

	ctx->funcTable = readFuncTable(fileBuff, &i);
	*ctx->ip = 0;

	return 0;
}

struct FuncTable *readFuncTable(char *fileBuff, unsigned int *pos) {
	int funcCount = *(int*)(fileBuff + *pos);

	// advance over funcCount
	*pos += sizeof(int);

	struct FuncTable *tbl = (struct FuncTable*)malloc(sizeof(struct FuncTable));
	tbl->table = (struct FuncHeader**)malloc(sizeof(struct FuncHeader*));

	for (int i = 0; i < funcCount; i++) {
		tbl->table[i] = (struct FuncHeader*)malloc(sizeof(struct FuncHeader));
		tbl->table[i]->returnType = *(char*)(fileBuff + *pos);
		*pos += 1;

		tbl->table[i]->paramTable = readParamTable(fileBuff, pos);
		tbl->table[i]->variableTable = readVariableTable(fileBuff, pos);
		tbl->table[i]->paramCount = tbl->table[i]->paramTable->tableCount;
		tbl->table[i]->variableCount = tbl->table[i]->variableTable->tableCount;

		tbl->table[i]->opCount = *(int*)(fileBuff + *pos);
		*pos += sizeof(int);
		tbl->table[i]->codeSize = *(int*)(fileBuff + *pos);
		*pos += sizeof(int);
		tbl->table[i]->code = (fileBuff + *pos);
		*pos += tbl->table[i]->codeSize;
	}

	tbl->tableCount = (unsigned int)funcCount;
	return tbl;
}

struct ParamTable *readParamTable(char *fileBuff, unsigned int *pos) {
	struct ParamTable *tbl = (struct ParamTable*)malloc(sizeof(struct ParamTable));
	tbl->table = (struct ParamHeader**)malloc(sizeof(struct ParamHeader*));

	tbl->tableCount = *(unsigned int*)(fileBuff + *pos);
	*pos += sizeof(unsigned int);

	for (int i = 0; i < tbl->tableCount; i++) {
		tbl->table[i] = (struct ParamHeader*)malloc(sizeof(struct ParamHeader));
		tbl->table[i]->index = i;
		tbl->table[i]->typeContainer = *(int*)(fileBuff + *pos);
		*pos += 4;
	}

	return tbl;
}

struct VariableTable *readVariableTable(char *fileBuff, unsigned int *pos) {
	struct VariableTable *tbl = (struct VariableTable*)malloc(sizeof(struct VariableTable));
	tbl->table = (struct VariableHeader**)malloc(sizeof(struct VariableHeader*));

	tbl->tableCount = *(unsigned int*)(fileBuff + *pos);
	*pos += sizeof(unsigned int);

	for (int i = 0; i < tbl->tableCount; i++) {
		tbl->table[i] = (struct VariableHeader*)malloc(sizeof(struct VariableHeader));
		tbl->table[i]->index = i;
		tbl->table[i]->typeContainer = *(int*)(fileBuff + *pos);
		*pos += 4;
	}

	return tbl;
}