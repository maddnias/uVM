// uVM.Bootstrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "uVM.Bootstrapper.h"
#include "Metadata.h"
#include "Executor.h"
#include "ErrorCodes.h"

struct RuntimeContext *initializeRuntimeFromFile(char *filePath);
void executeImpl(struct RuntimeContext *ctx);

int main(int argc, char *argv[]) {
	struct RuntimeContext *ctx = initializeRuntimeFromFile(argv[1]);
	executeImpl(ctx);
	return 0;
}

DLL_EXPORT void execute(char *fileBuff) {
	struct RuntimeContext *ctx = createRuntimeContext(NULL);
	readMetadata(ctx, fileBuff);
	executeImpl(ctx);
}

void executeImpl(struct RuntimeContext *ctx) {
	// Locate entrypoint and execute it
	struct FuncHeader *epHdr = ctx->funcTable->table[0];
	struct FunctionContext *func = createFunction(epHdr, ctx);
	executeFunction(func, ctx);
#ifdef _DEBUG
	getchar();
#endif
}

struct RuntimeContext *initializeRuntimeFromFile(char *filePath) {
	struct RuntimeContext *ctx = createRuntimeContext(filePath);
	struct FILE *file = fopen(ctx->file, "rb");

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	//fseek(file, 0, SEEK_SET);
	rewind(file);

	char* fileBuff = (char*)malloc(fileSize);
	fread(fileBuff, 1, fileSize, file);

	readMetadata(ctx, fileBuff);
	return ctx;
}

