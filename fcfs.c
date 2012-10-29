#include "os-project.h"

/**First Come First Served algorithm**/
void fcfs_algorithm(PROCESS* processes)
{
	int currentTime = 0;
	PROCESS *pp = processes;
	for (int i = 0; i < nfiles; i++,pp++) { //Work through elements.
		timeSoFar = max(timeSoFar, pp->stime);
		// time to print
		while(nextTimeToDumpIndex < nToDumps && timesToTakeDumps[nextTimeToDumpIndex] <= timeSoFar)
		{
			if (lf)
				fprintf(logger,"Taking time dump at time %d\n",
						tmpTime);
			dumpCacheToStream(memoryDumpStream);
			dumpMainMemoryToStream(memoryDumpStream);
			nextTimeToDumpIndex++;
		}
		if (pp->stime > currentTime) currentTime = pp->stime; //Increment time as nothing can run.
		if (memoryManage) {
			if (lf) 
				fprintf(logger,"Starting process %d, at time %d\n",i,currentTime);
			if (lf)
				fprintf(logger,"Once off load into main memory; first 4 lines\n");
			
			//load the first four lines (if they exist) into the main memory for no cost to the time
			loadIntoMainMemory(pp,1);
			if(pp->nlines >= 3)
			{
				loadIntoMainMemory(pp,3);
			}
			//run this process to completion, indicated by flag of -1
			timeSoFar += runProcessInTimeSlice(pp, -1);
		}
	}
	
	if (lf)
		fprintf(logger,"Scheduling finish time: %d\n",currentTime);
}	
