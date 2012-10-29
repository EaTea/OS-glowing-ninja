#include "os-project.h"

/**Round Robin Algorithm*/
void rr_algorithm(PROCESS* processes, int tq)
{
	QUEUE* q = new_queue(nfiles);

	timeSoFar = processes->stime;
	PROCESS *f = NULL; //the current front process
	PROCESS *pp = processes;
	//Allocate memory for scheduled time slots for each process
	//TODO: Move this to dynamically reallocate as we do not know the amount 
	//of timeslots at runtime.
	/*for (i = 0; i < nfiles; i++,pp++) {
		int nSlots = (pp->runningTime+1)/tq;
		pp->scheduledTimeSlots = malloc(nSlots*sizeof(int));
	}*/
	int i = 0;
	pp = processes;
	enqueue(q,pp);
	++i;
	++pp;
	
	
	while (i < nfiles || !is_empty(q)) {
		
		//Start of a quantum cycle
		//Enqueue any elements that are ready to start
		while (i < nfiles && pp->stime <= timeSoFar) {
			
			enqueue(q,pp);
			if (lf) 
					fprintf(logger,"Time is %d, enqueuing process %d with " \
							"start time %d\n", timeSoFar, pp->num, pp->stime);
			pp++; i++;
		}
		
		//IF there are no elements currently in the queue to process, we 
		//should advance to the start of the next available process.
		if (is_empty(q)) {
			timeSoFar = pp->stime;
			continue;
		}
		
		//This diff variable will store the amount of time remaining in a
		//process
		
		//dequeue front element, and continue to process.
		f = dequeue(q);
		
		if (lf)
			fprintf(logger,"Working with Process %s.\n",f->pname);
		int diff = runProcessInTimeSlice(f, tq);
		//(f->durationTimeSlots)[f->nTimeSlots-1] = diff;
		timeSoFar += diff;
		//check if finished?
		if(f->curLine <= f->nlines)
			//enqueue the file
			enqueue(q,f);
		/*if ((diff = f->runningTime-f->currtime) <= tq) {
			printf("We could finish this now! Diff = %d :)\n",diff);
			timeSoFar += diff;
			f->currtime += diff;
			printf("Current time is now suddenly %d\n",timeSoFar);
		} else {
			printf("Not ready to finish yet :(\n");
			f->currtime += tq;
			timeSoFar += tq;
			enqueue(&q,f);
		}*/
		
	}
	fprintf(logger,"Scheduling finish time: %d\n",timeSoFar);
	
}
