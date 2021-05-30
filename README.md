# SkeletonC-lightDSP

## Idea

et posteris judicas ...

My idea is to put together the basic tools to create a skeleton project in C. Through skeleton the user can be immediately operational, he will have:
- Scheduler
- Logger
- Circular buffer
- JSON config
- lightDSP (FFT iFFT Statistics)

Time permitting, I'll add more ...
MD Syntax Guide: https://www.markdownguide.org/basic-syntax/ 

## Architecture

Available architectures:
- Linux (x86_64)
- Arduino MCU (to do)
- ST MCU (to do)

### Linux Requirements
```
apt install texinfo build-essential manpages-dev make
```

## Compile and Run

Compile instruction (skeleton & DSP)
```
make clean
make 
./main
```

## How does it work ?

### Conventions
Function returns (int):
 - (0)   -> Success
 - (-1)  -> Error
<br/><br/>

### Main (main.c)

**lib: main.c and main.h**

It can be used to test all skeleton components.
Inside you will find 2 tasks with two different periods, one pushes and the other pops on a circular test buffer and it is possible to debug through the logger.
The comments explain how the library APIs work.
All API is masked by DEFINE Macro.
<br/><br/>

### Scheduler

**lib: scheduler.c and include/scheduler.h**

API:
```c
// name task, period is milliseconds, function call - return pid
int PROCESS_ATTACH(char *name, uint32_t period, void *function); 

// pid of the task - return successful or not
int PROCESS_DETACH(uint16_t pid);

// return successful or not - contains while(1)
int RUN();
```

Tricks: 
```c
usleep(MIN_TIMESLOT_NS); // 1 millisecond for CPU relax into "scheduler()"
```
<br/>

### Logger

**lib: logger.c and include/logger.h**

API:
```c
#define LOGGER_LEVEL_TRACE          LOW
#define LOGGER_LEVEL_DEBUG          ...
#define LOGGER_LEVEL_INFO           ...
#define LOGGER_LEVEL_WARNING        ...
#define LOGGER_LEVEL_ERROR          ...
#define LOGGER_LEVEL_FATAL          HIGH

// file name and path, logger level start, stdout enable/disable - return successful or not, [MUST] put on the main() head 
int LOGGER_INIT("test.log", LOGGER_LEVEL_DEBUG, LOGGER_STDOUT_ON);

// using like classic printf - return successful or not
int LOGGER_ERROR("color error\n");
int LOGGER_WARN("color warning\n");
int LOGGER_INFO("color infomation\n");
int LOGGER_DEBUG("color debug\n");

// close file, auto flush is always present for every logger raw - return successful or not
int LOGGER_CLOSE();
```
<br/>

### Circular Buffer
**lib: cbuf.c and include/cbuf.h**

API:
```c
// Defines a global circular buffer `buf` of a given type and size. The type
// can be native data types or user-defined data types.
// Usage:
// CIRC_GBUF_DEF(uint8_t, byte_buf, 13);
// CIRC_GBUF_DEF(struct foo, foo_buf, 10);
int CIRC_GBUF_DEF(type, buf, size);

// Resets the circular buffer offsets to zero. Does not clean the newly freed slots. 
int CIRC_GBUF_FLUSH(buf);

// Pushes element pointed to by `elem` at the head of circular buffer `buf`.
// This is read-write method, occupancy count increases by one.
int CIRC_GBUF_PUSH(buf, elem)  _push_refd(elem);

// Copies the element at tail of circular buffer `buf` into location pointed
// by `elem`. This method is read-only, does not later occupancy status. 
int CIRC_GBUF_POP(buf, elem)  _pop_refd(elem);

// Returns the number of free slots in the circular buffer `buf` - return: number of slots available.
int CIRC_GBUF_FS(buf)   __circ_gbuf_free_space(&buf);

// OPTIONS:
// Zero slots in circular buffer after a pop.
int CRICBUF_CLEAN_ON_POP
```
<br/>

### JSON Config

```c
// file name and path, config JSON File to read - return successful or not
int CONFIG_INIT("pathfile");

// put the key and read the value from JSON File
int CONFIG_READ(const char *key, char *value);

```
<br/>

## TIPS
Visual Studio Code
CMD+SHIFT+V -> .MD Preview on/off
Probably need to install something

# Have fun!
REM said, remember, the ticket to the future is always blank.<br/>
Vash
