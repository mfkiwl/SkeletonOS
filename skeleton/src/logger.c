#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include "logger.h"
#include "arch.h"

/**
 * global logger context
 */
static logger_ctx_t g_logger;

static uint8_t loggerReady;

int logger_init(char *filename, uint16_t options, uint16_t stdoutEnable)
{
	if (filename == NULL)
	{
		g_logger.storage = NO;
	}
	else
	{
		g_logger.storage = YES;
		memcpy(g_logger.filename, filename, strlen(filename));
	}

	g_logger.with_color = LOGGER_COLOR_OFF;
	g_logger.level = options & LOGGER_LEVEL_MASK;
	g_logger.stdoutEnable = stdoutEnable;

	if (1 == isatty(STDOUT_FILENO) && LOGGER_COLOR_ON == (options & LOGGER_COLOR_MASK))
	{
		g_logger.with_color = LOGGER_COLOR_ON;
	}

	loggerReady = 1;

	return 0;
}

int logger_printf(uint16_t log_level, const char *color, const char *format, ...)
{

	int nwritten = 0;
	va_list arg;

	if (log_level < g_logger.level)
	{
		return 0;
	}

	static char dataTime[64]; 
	getFormattedTime(dataTime);

	static char messageLogger[1024];
	static char args[512];

	va_start(arg, format);
	nwritten += vsprintf(args, format, arg);
	va_end(arg);

	sprintf(messageLogger, "%s %s \r\n", dataTime, args);

// TODO: switch
	if (g_logger.stdoutEnable == LOGGER_SERIAL_PRINT)
	{
		serialWriteString(messageLogger);
	}
	else if (g_logger.stdoutEnable == LOGGER_STDOUT_ON ) // linux
	{
		if (LOGGER_COLOR_ON == g_logger.with_color)
		{
			fprintf(stdout, "%s", color);
		}

		fprintf(stdout, "%s", messageLogger);

		if (LOGGER_COLOR_ON == g_logger.with_color)
		{
			fprintf(stdout, LOGGER_COLOR_RESET);
			fflush(stdout);
		}

	}
	else 
	{
		//continue
	}

	if (g_logger.storage == YES)
	{
		textFileWrite(g_logger.filename, messageLogger, APPEND);
	}

	return nwritten;
}
