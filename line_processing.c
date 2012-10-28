#include "os-project.h"

/*
 * Finds whether the current line is an IFLINE by searching the array of IFLINEs.
 * Accepts a pointer to the array of IFLINEs, and the current line being processed as inputs
 * Returns the index number in the IFLINEs array corresponding to the IFLINE on currentLine, or -1 if no such IFLINE exists
 */
static int IFLINESearch(IFLINE* iflines, int currentLine, int nIfs)
{
	for(int i = 0; i < nIfs; i++)
	{
		if(currentLine == iflines[i].originline)
			return i;
	}
	return -1;
}

int processLine(PROCESS* p, int* currentLine, int nIfs, int timeRemaining)
{
	IFLINE* iflines = p->iflines;
	//if there is an IFLINE find it
	int iflineIndex = IFLINESearch(iflines,*currentLine,nIfs);
	int retVal = 0;
	printf("InCache?%d\n",inCache(p,*currentLine));
	//if line in cache
	if(!memoryManage || inCache(p, *currentLine))
	{
		if(timeRemaining == -1 || timeRemaining >= 1)
		{
			//treat this like a normal line
			//relies on lazy evaluation
			if(iflineIndex == -1 || iflines[iflineIndex].looped >= iflines[iflineIndex].loopLimit)
				(*currentLine)++;
			else
			{
				*currentLine = iflines[iflineIndex].gotoline;
				iflines[iflineIndex].looped++;
			}

			//return the time taken to execute this step
			//in this case time step is value 1
			retVal = 1;
		}
	}
	//not in cache
	else
	{
		FRAME* f1 = NULL;
		FRAME* f2 = NULL;
		//inMainMemory does a pass by reference and returns the FRAME*s
		int foundLine = inMainMemory(p, *currentLine, &f1);
		int foundLine2 = (*currentLine+2 > p->nlines || inMainMemory(p,*currentLine+2,&f2));
		//if BOTH frames were found, then four lines can be moved;
		//alternatively if only one page is required, then foundLine2 will still be true
		//one page is required iff we are loading the last or second last lines
		if(foundLine && foundLine2)
		{
			//sufficient time to move to cache
			if((timeRemaining == -1 || timeRemaining >= 2))
			{ 
				//current line not inside the cache; load into cache
				loadIntoCache(f1,f2);
				
				//treat this like a normal line
				//relies on lazy evaluation
				if(iflineIndex == -1 || iflines[iflineIndex].looped >= iflines[iflineIndex].loopLimit)
					(*currentLine)++;
				else
				{
					*currentLine = iflines[iflineIndex].gotoline;
					iflines[iflineIndex].looped++;
				}

				//return the time taken to execute this step
				//in this case time step is value 2
				//(time cost of 1 to load and time cost of 1 to execute)
				retVal = 2;
			}
			else
			{
				//not enough time to load from main into cache, return 0
				return 0;
			}
		}
		//not in main memory; page fault
		else
		{
			//page fault!
			if(!foundLine)
				//pagefault is for the first two lines
				retVal = -1;
			else if(!foundLine2)
				//pagefault is for the last two lines
				retVal = -2;
		}
	}
	return retVal;
}

int runProcessInTimeSlice(PROCESS* p, int timeslice)
{
	int overallTime = 0;
	if(!timeslice)
	{
		//TODO: Lo9g and kill
		//fprintf(
		exit(1);
	}
	//run process until completion
	if(timeslice == -1)
	{
		printf("%s\n",p->pname);
		while(p->curLine <= p->nlines)
		{
			//printf("%s\n", p->pname);
			//printf("%d, %d\n", p->curLine, p->nlines);
			//keep on running a process until it's finished
			//printf("Line:%d\n",p->curLine);
			int timeConsumed = processLine(p, &(p->curLine), p->nifs, timeslice);
			if(timeConsumed < 0)
			{
				//page fault
				if(timeConsumed == -1)
				{
					printf("Page Fault; missing the first 2 lines\n");
					loadIntoMainMemory(p,p->curLine);
				}
				else if(timeConsumed == -2)
				{
					printf("Page Fault; missing the last 2 lines\n");
					loadIntoMainMemory(p,p->curLine+2);
				}
			}
			else
			{
				printf("TimeSoFar:%d\n", overallTime);
				overallTime += timeConsumed;
			}
			//TODO
			//if memory dump should occur in between
			if(nextTimeToDumpIndex < nToDumps && timeSoFar+overallTime >= timesToTakeDumps[nextTimeToDumpIndex])
			{
				dumpCacheToStream(memoryDumpStream);
				dumpMainMemoryToStream(memoryDumpStream);
				if(nextTimeToDumpIndex < nToDumps)
					nextTimeToDumpIndex++;
			}

		}
	}
	else
	{
		//run the process inside a timeslice
		while(overallTime < timeslice && p->curLine <= p->nlines)
		{
			int timeConsumed = processLine(p, &(p->curLine), p->nifs, timeslice-overallTime);
			if(timeConsumed < 1)
			{
				//page fault
				if(timeConsumed == -1)
				{
					printf("Page Fault; missing the first 2 lines\n");
					loadIntoMainMemory(p,p->curLine);
				}
				else if(timeConsumed == -2)
				{
					printf("Page Fault; missing the last 2 lines\n");
					loadIntoMainMemory(p,p->curLine+2);
				}

				//if we need to take a dump and put it into the stream
				if(nextTimeToDumpIndex < nToDumps && timeSoFar+overallTime >= timesToTakeDumps[nextTimeToDumpIndex])
				{
					dumpCacheToStream(memoryDumpStream);
					dumpMainMemoryToStream(memoryDumpStream);
					if(nextTimeToDumpIndex < nToDumps)
						nextTimeToDumpIndex++;
				}
				//in this case, the processLine function has returned 0
				//OR
				//page fault
				break;
			}
			overallTime += timeConsumed;
		}
	}
	//TODO: Add in that scheduled time slots is updated
	//TODO: Verify this worked
	(p->nTimeSlots)++;
	p->scheduledTimeSlots = (int*)realloc(p->scheduledTimeSlots, p->nTimeSlots * sizeof(int));
	p->durationTimeSlots = (int*)realloc(p->durationTimeSlots, p->nTimeSlots * sizeof(int));
	p->scheduledTimeSlots[p->nTimeSlots-1] = timeSoFar;
	p->durationTimeSlots[p->nTimeSlots-1] = overallTime;
	return overallTime;
}
