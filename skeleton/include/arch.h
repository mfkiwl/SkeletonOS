#ifndef ARCH_H_
#define ARCH_H_

#include <inttypes.h>
#include <stdint.h>

// disk access
#define APPEND  0
#define SHOT    1

extern int textFileRead(char* filename, char* content);
extern int textFileWrite(char* filename, char* content, uint8_t mode);

extern int nonVolatileRead(char* name, void* content, int size);
extern int nonVolatileWrite(char* name, void* content, int size);

// scheduler
extern void hardwareSchedulerRun();

// serial debug
extern void serialWriteString(const char* string);
extern void serialWriteChar(const char ch);

// time
extern uint64_t getMicrosecTime();
extern uint64_t getMillisTime();
extern void getFormattedTime(char* formattedTime);

// random
extern uint32_t rnd();

// drivers
extern void initDrivers();
extern int SquareGenerator(int pid);

// error handling
extern void arch_exit(int code, char* msg_format, ...);
#define EXIT(CODE, MSG_FORMAT, ...) arch_exit(CODE, MSG_FORMAT, ##__VA_ARGS__)

#endif // !ARCH_H_
