#include "os-project.h"

/** A number of Auxiliary Functions used in multiple files, such as reading ints and running the "usage" **/


void usage() {
  printf("Usage: \'%s [OPTIONS] FCFS in.file\' or \'%s RR n in.file\n",progname,progname);
  puts("where in.file is a list of job files, and n is an integer representing a time quantum\n");
	puts("the following flags can be accepted:\n");
	puts("-m [PARAMETERS]\n\tFlags that memory management should be enabled, and [PARAMETERS] should be a list of space-separated numbers\n");
	printf("\t\tExample: \'%s -m 5 10 20 FCFS in.file\' invokes the program with memory management enabled, instructing the program to dump memory contents to memoryDump.out at times 5, 10 and 20\n", progname);
}

int isint(char *str) {
  while (*str) {
    if (!isdigit(*str)) return 0;
    else str++;
  } return 1;
}

int cmpByStartTime(const void* a, const void* b)
{
	PROCESS* elem1 = (PROCESS*) a;
	PROCESS* elem2 = (PROCESS*) b;
	return elem1->stime - elem2->stime;
}

int max(int a, int b)
{
	return a < b ? b : a;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

void computeProcessRunTime(PROCESS* p)
{
	int runTime = 0;
	int nLines = p->nlines;
	int nIfs = p->nifs;
	runTime += nLines;
	for(int ifCount = 0; ifCount < nIfs; ifCount++)
	{
		IFLINE* ifIterator = p->iflines+ifCount;
		runTime += ifIterator->loopLimit * (ifIterator->originline - ifIterator->gotoline + 1);
	}
	p->runningTime = runTime;
}
