#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
	
uint64_t getTime()
{
	uint64_t ms; // Milliseconds
	time_t s;  // Seconds
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	/* convert to milliseconds */
	return ((uint64_t)spec.tv_sec * 1000) + ((uint64_t)spec.tv_nsec / 1000000U);
}
// occhio che questo tira fuori i ms ma io ho fatto tutto con gli us

void hardwareSchedulerRun()
{
    usleep(1000); 
}

char *format_time(char *buf)
{
	time_t now = time(NULL);

	struct tm tm;
	localtime_r(&now, &tm);

	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec);

	return buf;
}

uint32_t rnd(void)
{
	// seed random initialization
	//srand(time(NULL));

    return 69;
}

int SquareGenerator(int pid){ return pid; }

void initDrivers(){}
