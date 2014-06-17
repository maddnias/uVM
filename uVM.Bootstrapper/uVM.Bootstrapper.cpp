// uVM.Bootstrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "uVM.Bootstrapper.h"
#include "Metadata.h"
#include "Executor.h"
#include "ErrorCodes.h"

RuntimeContext *initializeRuntimeFromFile(char *filePath);
void executeImpl(RuntimeContext *ctx);

int main(int argc, char *argv[]) {
	RuntimeContext *ctx = initializeRuntimeFromFile(argv[1]);
	executeImpl(ctx);
	return 0;
}

DLL_EXPORT void execute(char *fileBuff) {
	RuntimeContext *ctx = createRuntimeContext(NULL);
	readMetadata(ctx, fileBuff);
	executeImpl(ctx);
}

void executeImpl(RuntimeContext *ctx) {
	// Locate entrypoint and execute it
	FuncHeader *epHdr = ctx->funcTable->table[0];
	FunctionContext *func = createFunction(epHdr, ctx);
	executeFunction(func, ctx);
}

RuntimeContext *initializeRuntimeFromFile(char *filePath) {
	RuntimeContext *ctx = createRuntimeContext(filePath);
	FILE *file = fopen(ctx->file, "rb");

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	//fseek(file, 0, SEEK_SET);
	rewind(file);

	char* fileBuff = (char*)malloc(fileSize);
	fread(fileBuff, 1, fileSize, file);

	readMetadata(ctx, fileBuff);
	return ctx;
}

