#include "hardware/watchdog.h"

uint64_t getTime()
{
    return time_us_64();
}

void hardwareSchedulerRun()
{
    watchdog_update();
}