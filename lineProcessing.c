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

int processLine(IFLINE* iflines, int* currentLine, int nIfs)
{
	int iflineIndex = IFLINESearch(iflines,*currentLine,nIfs);
	int retVal;
	if(inMainMemory(*currentLine))
	{
		if(inCache(*currentLine))
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
		else
		{
			//current line not inside the cache; load into cache
			//TODO: Implement this function
			//loadIntoCache(*currentLine);
			
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
	}
	else
	{
		//page fault
		retVal = -1;
	}
	return retVal;
}
