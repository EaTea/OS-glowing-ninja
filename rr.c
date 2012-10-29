#include "os-project.h"

/**Round Robin Algorithm*/
void rr_algorithm(PROCESS* processes, int tq)
{
	QUEUE q = new_queue(nfiles);

	timeSoFar = processes->stime;
	PROCESS *f = NULL; //the current front process
	PROCESS *pp = processes;

	int i = 0;
	pp = processes;
	enqueue(&q,pp);
	++i;
	++pp;
	
	
	while (i < nfiles || !is_empty(q)) {
		
		//Start of a quantum cycle
		//Enqueue any elements that are ready to start
		while (i < nfiles && pp->stime <= timeSoFar) {
			
			enqueue(&q,pp);
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
		f = dequeue(&q);
		if (memoryManage && f->nTimeSlots == 0) {
			if (lf) 
				fprintf(logger,"Starting process %d, at time %d\n",
						i,timeSoFar);
			//run this process to completion
			if (lf)
				fprintf(logger,"Once off load into main memory; first 4 lines\n");
			
			loadIntoMainMemory(pp,1);
			if(pp->nlines >= 3)
			{
				loadIntoMainMemory(pp,3);
			}
		}
		//TODO: This is where we should now ALLOCATE the new space instead of
		// just adding it to the next one.
		//++(f->nTimeSlots);
		//f->scheduledTimeSlots = (int*)realloc(f->scheduledTimeSlots, (f->nTimeSlots)*sizeof(int));
		//f->durationTimeSlots = (int*)realloc(f->durationTimeSlots, (f->nTimeSlots)*sizeof(int));
		//(f->scheduledTimeSlots)[f->nTimeSlots-1] = timeSoFar;
	
		if (lf)
			fprintf(logger,"Working with Process %s.\n",f->pname);
		int diff = runProcessInTimeSlice(f, tq);
		//(f->durationTimeSlots)[f->nTimeSlots-1] = diff;
		timeSoFar += diff;
		//check if finished?
		if(f->curLine <= f->nlines)
			//enqueue the file
			enqueue(&q,f);
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
