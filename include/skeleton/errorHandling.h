#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

#include <stdio.h>

#define EXIT(CODE, MSG_FORMAT, ...) fprintf(stderr, MSG_FORMAT, ##__VA_ARGS__), fflush(stderr), exit(CODE)

#endif