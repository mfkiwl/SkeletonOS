#ifndef ARCH_H_
#define ARCH_H_

#define RAND_MAX 32767

// Disk Write
#define APPEND  0
#define SHOT    1
int fileWrite(char* filename, char *message);


// Scheduler
extern void hardwareSchedulerRun();

// pioSerial
extern void initPIOSERIAL();
extern void writeString(const char* string);
extern void writeChar(const char ch);

// time
extern uint64_t getTime();
extern void getDataTime(char *);

// random
uint32_t rnd();

// drivers
extern void initDrivers();
extern int SquareGenerator(int pid);

#endif // !ARCH_H_
