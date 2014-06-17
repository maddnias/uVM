#include "stdafx.h"
#include "TypeContainer.h"

int getMainType(TypeContainer *container) {
	// high 16 bits
	return container->container >> 16;
}

int getSubType(TypeContainer *container) {
	// low 16 bits
	return container->container & MAXUINT16;
}
int hasSubType(TypeContainer *container) {
	return (getSubType(container) == getMainType(container) ? 0 : 1);
}