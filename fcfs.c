#include "os-project.h"

void fcfs_algorithm(PROCESS* processes, int nProcesses)
{
	qsort(processes,nProcesses,sizeof(PROCESS),cmpByStartTime);
	int currentTime = 0;
	for(int pId = 0; pId < nProcesses; pId++)
	{
		currentTime = max(processes[pId].stime, currentTime);
		processes[pId].nTimeSlots = 2;
		processes[pId].scheduledTimeSlots = realloc(processes[pId].scheduledTimeSlots,nTimeSlots * sizeof(int));
		processes[pId].scheduledTimeSlots[0] = currentTime;
		//TODO: make the function commented out below
		//runProcess(processes[pId], 0);
		processes[pId].scheduledTimeSlots[1] = currentTime + processes[pId].runningTime;
	}
}
