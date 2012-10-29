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

	//if line in cache
	if(!memoryManage || inCache(p, *currentLine))
	{
		if(timeRemaining == -1 || timeRemaining >= 1)
		{
			//treat this like a normal line
			//relies on lazy evaluation
			if(iflineIndex == -1 || iflines[iflineIndex].looped >= iflines[iflineIndex].loopLimit)
				(*currentLine)++;
			//an IFLINE! jump to the right place and increment loop counter
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
			//sufficient time to move to cache and run
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
		//not in main memory or cache; page fault
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
		fprintf(stderr,"Attempting to run program with a time quantum of 0 "\
					    "does not make sense.\n");
		usage();
		exit(1);
	}
	//run process until completion
	if(timeslice == -1)
	{
		if (lf) fprintf(logger,"Active process: %s\n",p->pname);
		while(p->curLine <= p->nlines)
		{
			//time consumed
			int timeConsumed = processLine(p, &(p->curLine), p->nifs, timeslice);
			//the total time taking into account how long this has run + the time consumed in processing the line above
			int tmpTime = timeSoFar + overallTime + max(timeConsumed,0);
			//should dump
			while(nextTimeToDumpIndex < nToDumps && tmpTime > timesToTakeDumps[nextTimeToDumpIndex])
			{
				if (lf)
					fprintf(logger,"Taking time dump at time %d\n",
							tmpTime);
				dumpCacheToStream(memoryDumpStream);
				dumpMainMemoryToStream(memoryDumpStream);
				nextTimeToDumpIndex++;
			}

			if(timeConsumed < 0)
			{
				//page fault
				if(timeConsumed == -1)
				{
					if (lf) 
						fprintf(logger,"Page Fault at time %d; missing the "\
								"first 2 lines\n",tmpTime);
					loadIntoMainMemory(p,p->curLine);
				}
				else if(timeConsumed == -2)
				{
					if (lf)
						fprintf(logger,"Page Fault at time %d; missing the "\
								"first 2 lines\n",tmpTime);
					loadIntoMainMemory(p,p->curLine+2);
				}
			}
			else
				overallTime += timeConsumed;
		}
	}
	else
	{
		//run the process inside a timeslice
		while(overallTime < timeslice && p->curLine <= p->nlines)
		{
			int timeConsumed = processLine(p, &(p->curLine), p->nifs, timeslice-overallTime);
			//if we need to take a dump and put it into the stream
			int tmpTime = timeSoFar + overallTime + max(timeConsumed,0);
			while(nextTimeToDumpIndex < nToDumps && tmpTime > timesToTakeDumps[nextTimeToDumpIndex])
			{
				if (lf)
					fprintf(logger,"Taking time dump at time %d\n",
							tmpTime);
				dumpCacheToStream(memoryDumpStream);
				dumpMainMemoryToStream(memoryDumpStream);
				nextTimeToDumpIndex++;
			}
			if(timeConsumed < 1)
			{
				//page fault
				if(timeConsumed == -1)
				{
					if (lf)
						fprintf(logger,"Page Fault at time %d; missing the "\
								"first 2 lines\n",tmpTime);
					loadIntoMainMemory(p,p->curLine);
				}
				else if(timeConsumed == -2)
				{
					if (lf)
						fprintf(logger,"Page Fault at time %d; missing the "\
								"last 2 lines\n",tmpTime);
					loadIntoMainMemory(p,p->curLine+2);
				}
				//in this case, the processLine function has returned 0
				//OR
				//page fault
				break;
			}
			overallTime += timeConsumed;
		}
	}
	//reallocate and increase the scheduled slots
	(p->nTimeSlots)++;
	p->scheduledTimeSlots = (int*)realloc(p->scheduledTimeSlots, p->nTimeSlots * sizeof(int));
	p->durationTimeSlots = (int*)realloc(p->durationTimeSlots, p->nTimeSlots * sizeof(int));
	p->scheduledTimeSlots[p->nTimeSlots-1] = timeSoFar;
	p->durationTimeSlots[p->nTimeSlots-1] = overallTime;
	return overallTime;
}
