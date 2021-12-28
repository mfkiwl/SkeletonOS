#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>

uint64_t getTimestamp();

#define GET_TIMESTAMP() \
	getTimestamp()

#endif