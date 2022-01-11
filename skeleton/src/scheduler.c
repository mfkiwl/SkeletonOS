#include <string.h>
#include <stdio.h>

#include "scheduler.h"
#include "logger.h"
#include "cmake.h"

#include "arch.h"

static struct pcb processlist[SCHEDULER_MAX_PROCESSES];

int process_attach(uint32_t period, void *function)
{
	uint8_t i = 0;
	int ret = -1;

	while (i < SCHEDULER_MAX_PROCESSES)
	{
		if (processlist[i].attached != 1)
		{
			LOGGER_DEBUG("attach process at %d", i);

			processlist[i].pid = i;
			processlist[i].period = period;
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

int scheduler()
{
	void (*p)(uint8_t pid);

	while (1)
	{
		uint64_t now = getMicrosecTime();

		for (uint16_t i = 0; i < SCHEDULER_MAX_PROCESSES; i++)
		{
			if (processlist[i].attached == 1)
			{
				if (now >= processlist[i].elapsed)
				{
					p = (void *)processlist[i].function;
					(*p)(processlist[i].pid);

					processlist[i].elapsed = now + processlist[i].period;
					LOGGER_TRACE("task: %i in time: %" PRIu64 "!", processlist[i].pid, now); //to do: possible error in ARM version
				}
			}
		}
		
		hardwareSchedulerRun();
	}

	return 0;
}