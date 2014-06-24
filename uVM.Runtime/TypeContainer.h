#include "stdafx.h"

#pragma once
struct TypeContainer {
	int container;
};

int getMainType(struct TypeContainer *container);
int getSubType(struct TypeContainer *container);
int hasSubType(struct TypeContainer *container);