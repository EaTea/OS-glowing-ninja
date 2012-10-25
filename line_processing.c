#include "os-project.h"

/*
 * Finds whether the current line is an IFLINE by searching the array of IFLINEs.
 * Accepts a pointer to the array of IFLINEs, and the current line being processed as inputs
 * Returns the index number in the IFLINEs array corresponding to the IFLINE on currentLine, or -1 if no such IFLINE exists
 */
static int IFLINESearch(IFLINE* iflines, int currentLine, int nIfs)
{	//TODO: make a binary search
	int l = 0, h = nIfs-1, m;
	
	while (h - l < 1) {
// 		puts("Good luck with your binary");
		m = (h-l)/2;
// 		printf("h=%d,m=%d,l=%d\n",h,m,l);
		if (iflines[m].originline == currentLine) return m;
		else if (iflines[m].originline > currentLine) h = m;
		else l = m;
	} 
	if (iflines[l].originline == currentLine) return l;
	if (iflines[h].originline == currentLine) return h;
	else return -1;
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
		{
			*currentLine = iflines[iflineIndex].gotoline;
			iflines[iflineIndex].looped++;
		}

		//return the time taken to execute this step
		//in this case time step is value 1
		retVal = 1;
	}
	else if(timeRemaining == -1 || timeRemaining >= 2)
	{ //current line not inside the cache; load into cache
		//TODO: Implement this function
		loadIntoCache(p, *currentLine);
		
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
			//printf("%s\n", p->pname);
			//printf("%d, %d\n", p->curLine, p->nlines);
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
	//TODO: Verify this worked
	(p->nTimeSlots)++;
	p->scheduledTimeSlots = realloc(p->scheduledTimeSlots, p->nTimeSlots);
	p->durationTimeSlots = realloc(p->durationTimeSlots, p->nTimeSlots);
	p->scheduledTimeSlots[p->nTimeSlots-1] = timeSoFar;
	p->durationTimeSlots[p->nTimeSlots-1] = overallTime;
	return overallTime;
}
