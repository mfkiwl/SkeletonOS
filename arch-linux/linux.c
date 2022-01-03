#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
	
uint64_t getTime()
{
	/* for use on POSIX machines */
	struct timeval tv;
	gettimeofday(&tv, NULL);

	/* convert to milliseconds */
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void hardwareSchedulerRun()
{
    usleep(1000); 
}
