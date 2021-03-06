#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdint.h>

/**
 * color definitions
 */
#define LOGGER_COLOR_RED 	"\x1b[31m"
#define LOGGER_COLOR_GREEN 	"\x1b[32m"
#define LOGGER_COLOR_YELLOW "\x1b[33m"
#define LOGGER_COLOR_BLUE 	"\x1b[34m"
#define LOGGER_COLOR_PURPLE "\x1b[35m"
#define LOGGER_COLOR_CYAN 	"\x1b[36m"
#define LOGGER_COLOR_WHITE 	"\x1b[37m"
#define LOGGER_COLOR_RESET 	"\x1b[0m"

#define LOGGER_COLOR_RED_BOLD 		"\x1b[31;1m"
#define LOGGER_COLOR_GREEN_BOLD 	"\x1b[32m"
#define LOGGER_COLOR_YELLOW_BOLD	"\x1b[33m"
#define LOGGER_COLOR_BLUE_BOLD 		"\x1b[34m"
#define LOGGER_COLOR_PURPLE_BOLD 	"\x1b[35m"
#define LOGGER_COLOR_CYAN_BOLD 		"\x1b[36m"
#define LOGGER_COLOR_WHITE_BOLD 	"\x1b[37m"

#define LOGGER_ON_SDCARD 	0
#define LOGGER_ON_DISK 		1

/**
 * logger context
 */
typedef struct logger_ctx_s
{

	char filename[128];

	#define LOGGER_LEVEL_TRACE 		0 << 1
	#define LOGGER_LEVEL_DEBUG 		1 << 1
	#define LOGGER_LEVEL_INFO 		2 << 1
	#define LOGGER_LEVEL_WARNING 	3 << 1
	#define LOGGER_LEVEL_ERROR 		4 << 1
	#define LOGGER_LEVEL_FATAL 		5 << 1
	#define LOGGER_LEVEL_MASK 		0x07 << 1
	uint8_t level;

	#define LOGGER_COLOR_OFF 	1
	#define LOGGER_COLOR_ON 	0
	#define LOGGER_COLOR_MASK 	0x01
	uint8_t with_color;

	#define LOGGER_SERIAL_PRINT 	2
	#define LOGGER_STDOUT_ON 		1
	#define LOGGER_STDOUT_OFF 		0
	uint8_t stdoutEnable;

	#define YES 	0
	#define NO 		1
	uint8_t storage;

} logger_ctx_t;

/**
 * Initialize logger
 * @param filename log file path&name
 * @param options for logger
 *   LOGGER_COLOR_ON: (default) enable colorful log, only for ternimal;
 *   LOGGER_COLOR_OFF: disable colorful log;
 *   LOGGER_LEVEL_DEBUG, LOGGER_LEVEL_INFO, LOGGER_LEVEL_WARNING, LOGGER_LEVEL_ERROR: define log level
 */
extern int logger_init(char *filename, uint16_t options, uint16_t stdoutEnable);

#define LOGGER(A, B, C) \
	logger_init(A, B, C)

/**
 * Record a log
 */
extern int logger_printf(uint16_t log_level, const char *color, const char *format, ...);

// level file_path:func_name:line_number
#define LOGGER_PREFIX "[%s] (%s:%s:%d) "

#define LOGGER_TRACE(format, ...) \
	logger_printf(LOGGER_LEVEL_TRACE, LOGGER_COLOR_CYAN, LOGGER_PREFIX format, "TRACE", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOGGER_DEBUG(format, ...) \
	logger_printf(LOGGER_LEVEL_DEBUG, LOGGER_COLOR_WHITE, LOGGER_PREFIX format, "DEBUG", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOGGER_INFO(format, ...) \
	logger_printf(LOGGER_LEVEL_INFO, LOGGER_COLOR_GREEN, LOGGER_PREFIX format, "INFO", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOGGER_WARNING(format, ...) \
	logger_printf(LOGGER_LEVEL_WARNING, LOGGER_COLOR_YELLOW, LOGGER_PREFIX format, "WARN", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOGGER_ERROR(format, ...) \
	logger_printf(LOGGER_LEVEL_ERROR, LOGGER_COLOR_RED, LOGGER_PREFIX format, "ERROR", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LOGGER_FATAL(format, ...) \
	logger_printf(LOGGER_LEVEL_FATAL, LOGGER_COLOR_RED_BOLD, LOGGER_PREFIX format, "FATAL", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#endif // !LOGGER_H
