#include "os-project.h"

/**First Come First Served algorithm**/
void fcfs_algorithm(PROCESS* processes)
{
	int currentTime = 0;
	PROCESS *pp = processes;
	for (int i = 0; i < nfiles; i++,pp++) { //Work through elements.
		timeSoFar = max(timeSoFar, pp->stime);
		if (pp->stime > currentTime) currentTime = pp->stime; //Increment time as nothing can run.

		if (lf) 
			fprintf(logger,"Starting process %d, at time %d\n",i,currentTime);
		//run this process to completion
		if (lf)
			fprintf(logger,"Once off load into main memory; first 4 lines\n");
		
		loadIntoMainMemory(pp,1);
		if(pp->nlines >= 3)
		{
			loadIntoMainMemory(pp,3);
		}
		timeSoFar += runProcessInTimeSlice(pp, -1);
	}
	
	if (lf)
		fprintf(logger,"Scheduling finish time: %d\n",currentTime);
}	
