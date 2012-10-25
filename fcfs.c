#include "os-project.h"

//TODO: Note that I've used nfiles as the number of processes. We need to fail gracefully if a file is invalid.
//This is mostly done.

/***/
void fcfs_algorithm(PROCESS* processes)
{
	qsort(processes,nfiles,sizeof(PROCESS),cmpByStartTime);
	if (lf) fprintf(logger,"Processes sorted\n");
	int currentTime = 0;
	//QUEUE q = new_queue(nProcesses);
	PROCESS *pp = processes;
	printf("nfiles = %d",nfiles);
	for (int i = 0; i < nfiles; i++,pp++) { //Work through elements.
		if (pp->stime > currentTime) currentTime = pp->stime; //Increment time as nothing can run.

		printf("Starting process %d, at time %d\n",i,currentTime);
		pp->nTimeSlots = 1;
		pp->scheduledTimeSlots = malloc(sizeof(int));
		(pp->scheduledTimeSlots)[0] = currentTime;
		currentTime += pp->runningTime;
	}
	
	printf("Scheduling finish time: %d\n",currentTime);
}	
