#include "os-project.h"

//TODO: Note that I've used nfiles as the number of processes. We need to fail gracefully if a file is invalid.
//This is mostly done.

/***/
void fcfs_algorithm(PROCESS* processes)
{
	//TODO: See if Queue is necessary for FCFS - Probably isn't.
	//We can implement this all in one hit as qsort guarantees that a process will start
	//at its earliest possible time, and in order of Start Time.
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
	
	
// 	for(int pId = 0; pId < nProcesses; pId++)
// 	{
// 		currentTime = max(processes[pId].stime, currentTime);
// 		processes[pId].nTimeSlots = 2;
// 		processes[pId].scheduledTimeSlots = realloc(processes[pId].scheduledTimeSlots,processes[pId].nTimeSlots * sizeof(int));
// 		processes[pId].scheduledTimeSlots[0] = currentTime;
// 		//TODO: make the function commented out below
// 		//runProcess(processes[pId], 0);
// 		processes[pId].scheduledTimeSlots[1] = currentTime + processes[pId].runningTime;
// 	}
}	
