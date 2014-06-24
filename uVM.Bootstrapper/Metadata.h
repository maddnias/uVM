#include "stdafx.h"
#include "uVM.Runtime.h"

int finalizeRuntimeContext(struct RuntimeContext *ctx);
int readMetadata(struct RuntimeContext *ctx, char *fileBuff);

#pragma once
struct FileHeader {
	int magic;
	int ep;
};