#include "stdafx.h"
#include "Metadata.h"
#include "ErrorCodes.h"

FuncTable *readFuncTable(char *fileBuff, unsigned int *pos);

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
		tbl->table[i]->paramCount = *(unsigned int*)(fileBuff + *pos);
		*pos += sizeof(unsigned int);

		if (tbl->table[i]->paramCount > 0) {
			tbl->table[i]->paramTypes = (char**)malloc(sizeof(char*));
			for (int j = 0; j < tbl->table[i]->paramCount; j++) {
				tbl->table[i]->paramTypes[j] = (char*)malloc(sizeof(char));
				tbl->table[i]->paramTypes[j] = (char*)(fileBuff + *pos);
				*pos += 1;
			}
		}

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
