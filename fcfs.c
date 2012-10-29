#include "os-project.h"

/**First Come First Served algorithm**/
void fcfs_algorithm(PROCESS* processes)
{
	qsort(processes,nfiles,sizeof(PROCESS),cmpByStartTime);
	if (lf) fprintf(logger,"Processes sorted\n");
	int currentTime = 0;
	//QUEUE q = new_queue(nProcesses);
	PROCESS *pp = processes;
	printf("nfiles = %d\n",nfiles);
	for (int i = 0; i < nfiles; i++,pp++) { //Work through elements.
		timeSoFar = max(timeSoFar, pp->stime);
		if (pp->stime > currentTime) currentTime = pp->stime; //Increment time as nothing can run.

		printf("Starting process %d, at time %d\n",i,currentTime);
		//run this process to completion
		printf("Once off load into main memory; first 4 lines\n");
		loadIntoMainMemory(pp,1);
		if(pp->nlines >= 3)
		{
			loadIntoMainMemory(pp,3);
		}
		timeSoFar += runProcessInTimeSlice(pp, -1);
	}
	
	printf("Scheduling finish time: %d\n",currentTime);
}	
