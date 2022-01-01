#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "scheduler.h"
#include "logger.h"
#include "cmake.h"

static struct pcb processlist[MAX_PROCESSES];

int process_attach(char *name, uint32_t period, void *function)
{
	uint16_t i = 0;
	int ret = -1;

	while (i < MAX_PROCESSES)
	{
		if (strlen(name) > MAX_NAME_LEN)
		{
			LOGGER_ERROR("wrong stringlen\n");
			return ret;
		}

		if (processlist[i].attached != 1)
		{
			LOGGER_DEBUG("attach process at %d\n", i);

			processlist[i].pid = i;

			memcpy(processlist[i].name, name, strlen(name) + 1);

			if (period < MIN_TIMESLOT_MS)
			{
				processlist[i].period = MIN_TIMESLOT_MS;
			}
			else
			{
				processlist[i].period = period;
			}

			processlist[i].function = function;
			processlist[i].attached = 1;

			ret = 0;
			break;
		}
		i++;
	}
	return ret;
}

int process_detach(uint16_t pid)
{
	processlist[pid].attached = 0;
	return 0;
}

/* returns the current time in microseconds with some arbitrary start point */
static uint64_t getTime(void)
{
#if (ARM_PLATFORM)
	/* just use the arduino epoch */
	return millis();
#elif (LINUX_PLATFORM)
	/* for use on POSIX machines */
	struct timeval tv;
	gettimeofday(&tv, NULL);

	/* convert to milliseconds */
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}

/*
 * basic implementation of a RR scheduler 
 */
int scheduler()
{
	void (*p)(void);

	while (1)
	{
		uint64_t now = getTime();

		for (uint16_t i = 0; i < MAX_PROCESSES; i++)
		{
			if (processlist[i].attached == 1)
			{
				if (now >= processlist[i].elapsed)
				{
					p = (void *)processlist[i].function;
					(*p)();

					processlist[i].elapsed = now + processlist[i].period;
					LOGGER_TRACE("Task: %i in time: %" PRIu64 "!\n", processlist[i].pid, now);
				}
			}
		}

		usleep(MIN_TIMESLOT_NS); // 100 millisecond for CPU relax
	}

	return 0;
}