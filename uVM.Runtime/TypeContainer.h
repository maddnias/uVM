#include "stdafx.h"

#pragma once
struct TypeContainer {
	int container;
};

int getMainType(TypeContainer *container);
int getSubType(TypeContainer *container);
int hasSubType(TypeContainer *container);