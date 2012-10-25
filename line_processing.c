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
	int iflineIndex = IFLINESearch(iflines,*currentLine,nIfs);
	int retVal;
	if(inCache(p, *currentLine) && (timeRemaining == -1 || timeRemaining >= 1))
	{
		//treat this like a normal line
		//relies on lazy evaluation
		if(iflineIndex == -1 || iflines[iflineIndex].looped >= iflines[iflineIndex].loopLimit)
			(*currentLine)++;
		else
			*currentLine = iflines[iflineIndex].gotoline;

		//return the time taken to execute this step
		//in this case time step is value 1
		retVal = 1;
	}
	else if(timeRemaining == -1 || timeRemaining >= 2)
	{
		//current line not inside the cache; load into cache
		//TODO: Implement this function
		loadIntoCache(p, *currentLine);
		
		//treat this like a normal line
		//relies on lazy evaluation
		if(iflineIndex == -1 || iflines[iflineIndex].looped >= iflines[iflineIndex].loopLimit)
			(*currentLine)++;
		else
			*currentLine = iflines[iflineIndex].gotoline;

		//return the time taken to execute this step
		//in this case time step is value 2
		//(time cost of 1 to load and time cost of 1 to execute)
		retVal = 2;
	}
	else
	{
		return 0;
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
		while(p->curLine <= p->nlines)
		{
			//keep on running a process until it's finished
			int timeConsumed = processLine(p, &(p->curLine), p->nifs, timeslice);
			if(timeConsumed < 0)
			{
				//page fault
				break;
			}
			overallTime += timeConsumed;
		}
	}
	else
	{
		while(overallTime < timeslice && p->curLine <= p->nlines)
		{
			int timeConsumed = processLine(p, &(p->curLine), p->nifs, timeslice-overallTime);
			if(timeConsumed < 1)
			{
				//in this case, the processLine function has returned 0
				//OR
				//page fault
				break;
			}
			overallTime += timeConsumed;
		}
	}
	//TODO: Add in that scheduled time slots is updated
	(p->nTimeSlots)++;
	p->scheduledTimeSlots = realloc(p->scheduledTimeSlots, p->nTimeSlots);
	p->durationTimeSlots = realloc(p->durationTimeSlots, p->nTimeSlots);
	p->scheduledTimeSlots[p->nTimeSlots-1] = timeSoFar;
	p->durationTimeSlots[p->nTimeSlots-1] = overallTime;
	return overallTime;
}