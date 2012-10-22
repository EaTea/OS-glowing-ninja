#include "os-project.h"
//TODO: Note that I've used nfiles as the number of processes. We need to fail gracefully if a file is invalid.
//This is mostly done.

/***/
void rr_algorithm(PROCESS* processes, int tq)
{
	QUEUE q = new_queue(nfiles);
	int currentTime = processes->stime, i = 0;
	PROCESS *f = NULL; //the current front process
	PROCESS *pp = processes;
	enqueue(&q,pp);
	++i;
	++pp;
	while (i < nfiles || !is_empty(q)) {
		
		printf("t=%d\n",currentTime);
		//Start of a quantum cycle
		//Enqueue any elements that are ready to start
		if (is_empty(q)) {
			currentTime = pp->stime;
		}
		while (i < nfiles && pp->stime >= currentTime) {
			printf("Are we attempting to segfault an enqueue? ?\n");
			enqueue(&q,pp);
			printf("Time is %d, enqueuing process %d with stime %d\n", currentTime,pp->num,pp->stime);
			pp++; i++;
		}
		
		int diff;
		if (f == NULL) {
			//dequeue front element, and continue to process.
			f = dequeue(&q);
			printf("Working with Process %d..\n",f->num);
			if ((diff = f->runningTime-f->currtime) < tq) {
				printf("We could finish this now! Diff = %d :)\n",diff);
				currentTime += diff;
				f->currtime += diff;
				printf("Current time is now suddenly %d\n",currentTime);
			} else {
				printf("Not ready to finish yet :(\n");
				f->currtime += tq;
				currentTime += tq;
				enqueue(&q,f);
			}
			f = NULL;
		}
		
		
		//currentTime++;
		
	}
	
}