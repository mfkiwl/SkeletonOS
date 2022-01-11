#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <inttypes.h>

/* Process control block - 
 * holding all process relevant informations 
*/
struct pcb
{
	uint16_t pid;			 /* ID of the proces */
	uint64_t period;		 /* process period */
	uint64_t elapsed;		 /* process time elapsed */
	uint32_t attached;		 /* 1 if attached to processlist, else 0 */
	uint64_t *function;		 /* pointer to the process function */
};

extern int process_attach(uint32_t period, void *function); // period is microseconds!
extern int process_detach(uint16_t pid);
extern int scheduler();

#define MILLISECONDS 1000
#define SECONDS 1000 * MILLISECONDS
#define MINUTES 60 * SECONDS

#define RUN() \
	scheduler()

#define PROCESS_ATTACH(PID, NAME) \
	process_attach(PID, NAME)

#define PROCESS_DETACH(PID) \
	process_detach(PID)

#endif // !SCHEDULER_H_