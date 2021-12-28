#include <time.h>

#include "timestamp.h"

uint64_t getTimestamp()
{
	uint64_t ms; // Milliseconds
	time_t s;  // Seconds
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	/* convert to milliseconds */
	return ((uint64_t)spec.tv_sec * 1000) + ((uint64_t)spec.tv_nsec / 1000000U);
}