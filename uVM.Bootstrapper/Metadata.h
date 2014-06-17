#include "stdafx.h"
#include "uVM.Runtime.h"

int finalizeRuntimeContext(RuntimeContext *ctx);
int readMetadata(RuntimeContext *ctx, char *fileBuff);

#pragma once
struct FileHeader {
	int magic;
	int ep;
};