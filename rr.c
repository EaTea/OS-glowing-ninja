#include "os-project.h"
//TODO: Note that I've used nfiles as the number of processes. We need to fail gracefully if a file is invalid.
//This is mostly done.

/***/
void rr_algorithm(PROCESS* processes, int tq)
{
	QUEUE q = new_queue(nfiles);
	int currentTime = processes->stime, i = 0;
	PROCESS *f = NULL;
	PROCESS *pp = processes;
	enqueue(&q,pp);
	while (i < nfiles || !is_empty(q)) {
		//Start of a quantum cycle
		if (f == NULL) {
			f= front(q);
		}
		
		
		
	}
	
}