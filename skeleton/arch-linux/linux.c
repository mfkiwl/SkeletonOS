#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "arch.h"

// return usec
uint64_t getMicrosecTime()
{
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	/* convert to microseconds */
	return ((uint64_t)spec.tv_sec * 1000000) + ((uint64_t)spec.tv_nsec / 1000U);
}

// return msec
uint64_t getMillisTime()
{
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	/* convert to milliseconds */
	return ((uint64_t)spec.tv_sec * 1000) + ((uint64_t)spec.tv_nsec / 1000000U);
}

void hardwareSchedulerRun()
{
  usleep(1000); 
}

void getFormattedTime(char* formattedTime)
{
	time_t now = time(NULL);

	struct tm tm;
	localtime_r(&now, &tm);

	sprintf(formattedTime, "%04d-%02d-%02d %02d:%02d:%02d",
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec);

}

uint32_t rnd(void)
{
	return rand();
}

int SquareGenerator(int pid)
{
	return pid;
}

void initDrivers()
{
	// initialize seed for random
	srand(time(NULL));
}

void serialWriteString(const char* string)
{}

void serialWriteChar(const char ch)
{}

int textFileWrite(char* filename, char* content, uint8_t mode)
{
	FILE *fp;

	if (mode == APPEND)
		fp = fopen(filename, "a");
	else if (mode == SHOT)
		fp = fopen(filename, "w");

	if (fp == NULL)
	{
		return 1;
	}

	fprintf(fp, "%s", content);

	fclose(fp);

	return 0;
}

int textFileRead(char* filename, char* content)
{
	FILE *fp;
	uint64_t size;

	fp = fopen(filename, "rb");
	if (!fp)
	{
		EXIT(-1, "text file open error\n");
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	/* copy the file into the buffer */
	if (1 != fread(content, size, 1, fp))
		fclose(fp), free(content), EXIT(-1, "text file entire read fails\n");

	fclose(fp);

	return 0;
}

int nonVolatileRead(char* name, void* content, int size)
{
	FILE *fp;

	fp = fopen(name, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "problems while reading on non volatile memory: %s not present\n", name);
		fflush(stderr);
		return -1;
	}

	while (!feof(fp))
	{
		fread(content, size, 1, fp);
	}

	fclose(fp);

	return 0;
}

int nonVolatileWrite(char* name, void* content, int size)
{
	int rc;
	FILE *fp;

	fp = fopen(name, "w");
	if (fp == NULL)
	{
		fprintf(stderr, "problems while writing on non volatile memory: %s\n", name);
		fflush(stderr);
		return -1;
	}

	rc = fwrite(content, size, 1, fp);
	fclose(fp);

	return rc;
}

void arch_exit(int code, char* msg_format, ...)
{
	va_list args;
	va_start(args, msg_format);
	vfprintf(stderr, msg_format, args);
	fflush(stderr);
	va_end(args);
	exit(code);
}