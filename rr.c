#include "os-project.h"

/**Round Robin Algorithm*/
void rr_algorithm(PROCESS* processes, int tq)
{
	QUEUE* q = new_queue(nfiles);

	timeSoFar = processes->stime;
	PROCESS *f = NULL; //the current front process
	PROCESS *pp = processes;

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
			
			while(nextTimeToDumpIndex < nToDumps && timesToTakeDumps[nextTimeToDumpIndex] <= timeSoFar)
			{
				if (lf)
					fprintf(logger,"Taking time dump at time %d\n",
							tmpTime);
				dumpCacheToStream(memoryDumpStream);
				dumpMainMemoryToStream(memoryDumpStream);
				nextTimeToDumpIndex++;
			}
			timeSoFar = pp->stime;
			continue;
		}
		
		//This diff variable will store the amount of time remaining in a
		//process
		
		//dequeue front element, and continue to process.
		f = dequeue(q);
		if (memoryManage && f->nTimeSlots == 0) {
			if (lf) 
				fprintf(logger,"Starting process %d, at time %d\n",
						i,timeSoFar);
			//run this process to completion
			if (lf)
				fprintf(logger,"Once off load into main memory; first 4 lines\n");
			
			//load to memory first four lines
			loadIntoMainMemory(f,1);
			if(pp->nlines >= 3)
			{
				loadIntoMainMemory(f,3);
			}
		}

		if (lf)
			fprintf(logger,"Working with Process %s.\n",f->pname);
		int diff = runProcessInTimeSlice(f, tq);
		//(f->durationTimeSlots)[f->nTimeSlots-1] = diff;
		timeSoFar += diff;
		//check if finished
		if(f->curLine <= f->nlines)
			//enqueue the file
			enqueue(q,f);
	}
	fprintf(logger,"Scheduling finish time: %d\n",timeSoFar);
	
}
