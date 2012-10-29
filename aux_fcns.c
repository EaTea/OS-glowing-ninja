#include "os-project.h"

/** 
 * A number of Auxiliary Functions used in multiple files, such as reading 
 * ints and running the "usage" 
 **/


void usage() {
	printf("Usage: \'%s [OPTIONS] FCFS in.file\' or \'%s RR n in.file\n",
		   progname,progname);
	puts("where in.file is a list of job files, and n is an integer "\
		 "representing a time quantum\n");
	puts("the following flags can be accepted:\n");
	puts("-m [PARAMETERS]\n\tFlags that memory management should be "\
		 "enabled, and [PARAMETERS] should be a list of "\
		 "space-separated numbers and a string out.file\n");
	printf("\t\tExample: \'%s -m 5 10 20 FCFS in.file out.file\' \ninvokes the program "\
		   "with memory management enabled, instructing the program to dump "\
		   "memory contents to out.file at times 5, 10 and 20\n",progname);
}

int isint(char *str) {
  while (*str) {
		//note: negative numbers are not accounted for
    if (!isdigit(*str)) return 0;
    else str++;
  } return 1;
}

int cmpByStartTime(const void* a, const void* b)
{
	//comparator for a qsort; see qsort's man page for more information
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

/*	Prints the process schedule to Stdout 
 *	To the tune of PSY's Gangnam Style
 *	http://www.youtube.com/watch?v=9bZkp7q19f0
 */
void print_schedule(PROCESS *ps) {
	printf("\nResult of Schedule:\n");
	for (int y = 0; y < nfiles; y++) {
		//HEEEEY SEXY LADY
		printf("%s:\t",ps[y].pname);
		//OP OP
		for (int j = 0; j < ps[y].nTimeSlots; j++) {
			//OPPA GANGNAM STYLE!
			printf("(%d, %d)\t",ps[y].scheduledTimeSlots[j],
			ps[y].durationTimeSlots[j]);
		} printf("\n");
		//OP OP
		printf("File contents:\n");
		for(int j = 0; j < ps[y].nlines; j++)
		{
			//OPPA GANGNAM STYLE!
			printf("\t%s\n", ps[y].lines[j]);
		}
		printf("\n");
	}
}

/*	Clears the memory associated with storing a process
 *	To the sweet, sweet sound of PSY's Gangnam Style
 *	http://www.youtube.com/watch?v=9bZkp7q19f0
 */
void clearProcesses(PROCESS* ps, int n)
{
	if(lf) fprintf(logger,"Freeing %d files\n",n);
	for (int y = 0; y < n; y++) {
		//HEEEEY SEXY LADY
		free(ps[y].pname);
		//OP OP
		free(ps[y].scheduledTimeSlots);
		//OP OP
		free(ps[y].durationTimeSlots);
		//OPPA GANGNAM STYLE
		free(ps[y].iflines);
	}
}
