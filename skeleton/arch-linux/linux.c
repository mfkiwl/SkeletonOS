#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// return usec
uint64_t getTime()
{
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	/* convert to milliseconds */
	return ((uint64_t)spec.tv_sec * 1000000) + ((uint64_t)spec.tv_nsec / 1000U);
}

void hardwareSchedulerRun()
{
    usleep(1000); 
}

void getDataTime(char *buf)
{
	time_t now = time(NULL);

	struct tm tm;
	localtime_r(&now, &tm);

	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec);

}

uint32_t rnd(void)
{
	// seed random initialization
	return rand();
}

int SquareGenerator(int pid){ return pid; }

void initDrivers()
{
	srand(time(NULL));
}

void writeString(const char* string){}

int fileWrite(char* filename, char *message)
{
	FILE *fp;

	fp = fopen(filename, "a");
	if (fp == NULL)
	{
		return 1;
	}

	fprintf(fp, "%s", message);

	fclose(fp);

	return 0;
}