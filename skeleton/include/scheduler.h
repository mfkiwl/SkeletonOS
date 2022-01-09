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

#define RUN() \
	scheduler()

#define PROCESS_ATTACH(A, B) \
	process_attach(A, B)

#define PROCESS_DETACH(A) \
	process_detach(A)

#endif // !SCHEDULER_H_