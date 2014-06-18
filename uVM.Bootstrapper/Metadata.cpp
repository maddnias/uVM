#include "stdafx.h"
#include "Metadata.h"
#include "ErrorCodes.h"

FuncTable *readFuncTable(char *fileBuff, unsigned int *pos);
ParamTable *readParamTable(char *fileBuff, unsigned int *pos);
VariableTable *readVariableTable(char *fileBuff, unsigned int *pos);

int finalizeRuntimeContext(RuntimeContext *ctx) {
	readMetadata(ctx, ctx->file);

	return 0;
}

int readMetadata(RuntimeContext *ctx, char *fileBuff) {
	unsigned int i = 0;

	FileHeader *fileHdr = (FileHeader*)(fileBuff);
	// advance over file header
	i += sizeof(FileHeader);

	ctx->funcTable = readFuncTable(fileBuff, &i);
	*ctx->ip = 0;

	return 0;
}

FuncTable *readFuncTable(char *fileBuff, unsigned int *pos) {
	int funcCount = *(int*)(fileBuff + *pos);

	// advance over funcCount
	*pos += sizeof(int);

	FuncTable *tbl = (FuncTable*)malloc(sizeof(FuncTable));
	tbl->table = (FuncHeader**)malloc(sizeof(FuncHeader*));

	for (int i = 0; i < funcCount; i++) {
		tbl->table[i] = (FuncHeader*)malloc(sizeof(FuncHeader));
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

ParamTable *readParamTable(char *fileBuff, unsigned int *pos) {
	ParamTable *tbl = (ParamTable*)malloc(sizeof(ParamTable));
	tbl->table = (ParamHeader**)malloc(sizeof(ParamHeader*));

	tbl->tableCount = *(unsigned int*)(fileBuff + *pos);
	*pos += sizeof(unsigned int);

	for (int i = 0; i < tbl->tableCount; i++) {
		tbl->table[i] = (ParamHeader*)malloc(sizeof(ParamHeader));
		tbl->table[i]->index = i;
		tbl->table[i]->typeContainer = *(int*)(fileBuff + *pos);
		*pos += 4;
	}

	return tbl;
}

VariableTable *readVariableTable(char *fileBuff, unsigned int *pos) {
	VariableTable *tbl = (VariableTable*)malloc(sizeof(VariableTable));
	tbl->table = (VariableHeader**)malloc(sizeof(VariableHeader*));

	tbl->tableCount = *(unsigned int*)(fileBuff + *pos);
	*pos += sizeof(unsigned int);

	for (int i = 0; i < tbl->tableCount; i++) {
		tbl->table[i] = (VariableHeader*)malloc(sizeof(VariableHeader));
		tbl->table[i]->index = i;
		tbl->table[i]->typeContainer = *(int*)(fileBuff + *pos);
		*pos += 4;
	}

	return tbl;
}