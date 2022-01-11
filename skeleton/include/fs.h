#ifndef FS_H_
#define FS_H_

#include "arch.h"

#define FS_WRITE(NAME, CONTENT, SIZE) \
	nonVolatileWrite(NAME, CONTENT, SIZE)

#define FS_READ(NAME, CONTENT, SIZE) \
	nonVolatileRead(NAME, CONTENT, SIZE)

#endif // !FS_H_