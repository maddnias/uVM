#include "stdafx.h"
#include "TypeContainer.h"

int getMainType(struct TypeContainer *container) {
	// high 16 bits
	return container->container >> 16;
}

int getSubType(struct TypeContainer *container) {
	// low 16 bits
	return container->container & MAXUINT16;
}
int hasSubType(struct TypeContainer *container) {
	return (getSubType(container) == getMainType(container) ? 0 : 1);
}