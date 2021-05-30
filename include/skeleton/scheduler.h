#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <inttypes.h>

#define MAX_PROCESSES 32 // the maximal number of processes in the system
#define MAX_NAME_LEN 32
#define MIN_TIMESLOT_NS 1000					 // nanosecons (1 milliseconds)
#define MIN_TIMESLOT_MS (MIN_TIMESLOT_NS / 1000) // milliseconds conversion

/* Process control block - 
 * holding all process relevant informations 
*/
struct pcb
{
	uint32_t pid;			 /* ID of the proces */
	uint32_t period;		 /* process period */
	uint64_t elapsed;		 /* process time elapsed */
	uint32_t attached;		 /* 1 if attached to processlist, else 0 */
	uint64_t *function;		 /* pointer to the process function */
	char name[MAX_NAME_LEN]; /* Name of the process */
};

extern int process_attach(char *name, uint32_t period, void *function); // period is milliseconds!
extern int process_detach(uint16_t pid);
extern int scheduler();

#define SECONDS 1000
#define MINUTES 60 * SECONDS

#define RUN() \
	scheduler()

#define PROCESS_ATTACH(A, B, C) \
	process_attach(A, B, C)

#define PROCESS_DETACH(A) \
	process_detach(A)

#endif // !SCHEDULER_H_