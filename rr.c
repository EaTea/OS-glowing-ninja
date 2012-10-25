#include "os-project.h"

/**Round Robin Algorithm*/
void rr_algorithm(PROCESS* processes, int tq)
{
	QUEUE q = new_queue(nfiles);
	int currentTime = processes->stime, i = 0;
	PROCESS *f = NULL; //the current front process
	PROCESS *pp = processes;
	//Allocate memory for scheduled time slots for each process
	//TODO: Move this to dynamically reallocate as we do not know the amount 
	//of timeslots at runtime.
	for (i = 0; i < nfiles; i++,pp++) {
		int nSlots = (pp->runningTime+1)/tq;
		pp->scheduledTimeSlots = malloc(nSlots*sizeof(int));
	}
	i = 0;
	pp = processes;
	enqueue(&q,pp);
	++i;
	++pp;
	
	
	while (i < nfiles || !is_empty(q)) {
		
		
		printf("t=%d\n",currentTime);
		//Start of a quantum cycle
		//Enqueue any elements that are ready to start
		while (i < nfiles && pp->stime <= currentTime) {
			
			enqueue(&q,pp);
			printf("Time is %d, enqueuing process %d with stime %d\n", 
				   currentTime,pp->num,pp->stime);
			pp++; i++;
		}
		
		//IF there are no elements currently in the queue to process, we 
		//should advance to the start of the next available process.
		if (is_empty(q)) {
			currentTime = pp->stime;
			continue;
		}
		
		//This diff variable will store the amount of time remaining in a
		//process
		int diff;
		
		//dequeue front element, and continue to process.
		f = dequeue(&q);
		
		//TODO: This is where we should now ALLOCATE the new space instead of
		// just adding it to the next one.
		(f->scheduledTimeSlots)[f->nTimeSlots] = currentTime;
		++(f->nTimeSlots);
	
		printf("Working with Process %d..\n",f->num);
		if ((diff = f->runningTime-f->currtime) <= tq) {
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
		
	}
	printf("Scheduling finish time: %d\n",currentTime);
	
}
