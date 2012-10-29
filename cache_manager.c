#include "os-project.h"


static char **pageOne;
static char **pageTwo;

//cache constants
static int pgOneStart;
static char pgOnePName[BUFSIZ];
static int pgTwoStart;
static char pgTwoPName[BUFSIZ];

void initialiseCache()
{
	if (lf) fprintf(logger,"Initialising cache\n");
	//a magic value; this value is safe and represents that the cache contains nothing
	pgOneStart = pgTwoStart = -4;
	//the strlen will now be 0, so should be guaranteed to match no process' name
	pgOnePName[0] = '\0';
	pgTwoPName[0] = '\0';
	//set the pointers to NULL; safeguard to explictly ensure that it isn't pointing to some random memory (only an issue when reallocing)
	pageOne = pageTwo = (char**) NULL;
	pageOne = calloc(sizeof(char*),2);
	pageOne[0] = calloc(sizeof(char),BUFSIZ);
	pageOne[1] = calloc(sizeof(char),BUFSIZ);
	pageTwo = calloc(sizeof(char*),2);
	pageTwo[0] = calloc(sizeof(char),BUFSIZ);
	pageTwo[1] = calloc(sizeof(char),BUFSIZ);
}

void tearDownCache()
{
	free(pageOne[0]);
	free(pageOne[1]);
	free(pageTwo[0]);
	free(pageTwo[1]);
	free(pageOne);
	free(pageTwo);
}

int inCache(PROCESS* p, int lineNumber)
{
	//page one process is sitting inside memory
	if(!strcmp(p->pname, pgOnePName))
		//page one's startline matches the line number
		if(pgOneStart == lineNumber || pgOneStart+1 == lineNumber)
			return 1;
	//page two process is sitting inside memory
	if(!strcmp(p->pname, pgTwoPName))
		//page two's startline matches the line number
		if(pgTwoStart == lineNumber || pgTwoStart+1 == lineNumber)
			return 1;
	return 0;
}

void loadIntoCache(FRAME* f1, FRAME* f2)
{
	if(f1 == NULL)
	{
		//TODO: ERROR
		return;
	}
	//f1 is FRAME containing lines lineNumber, lineNumber+1
	//f2 is FRAME containing lines lineNumber+2, lineNumber+3
	//p1, p2 are f1, f2's parents resp.
	//assumes that you know the FRAME in main memory that has the files you needed
	//record this process as the new process in cache
	strcpy(pgOnePName, f1->pname);
	//record the line number of the cache
	pgOneStart = f1->lineStart;
	strcpy(pageOne[0],f1->page[0]);
	strcpy(pageOne[1],f1->page[1]);
	updateMainMemory(f1);
	//no secondary frame to load
	if(f2 != NULL)
	{
	//hit the end of process whilst attempting to load process into cache
	//record "INVALID LINE" if this occurs
		strcpy(pgTwoPName, f2->pname);
		pgTwoStart = f2->lineStart;
		strcpy(pageTwo[0],f2->page[0]);
		strcpy(pageTwo[1],f2->page[1]);
		updateMainMemory(f2);
	}
}

void dumpCacheToStream(FILE* stream)
{
	fprintf(stream, "Time: %d\n", timesToTakeDumps[nextTimeToDumpIndex]);
	fprintf(stream, "Cache: Page One\nProcess Cached: %s\n",pgOnePName);
	fprintf(stream, "%s\n%s\n", pageOne[0], pageOne[1]);
	fprintf(stream, "Cache: Page Two\nProcess Cached: %s\n",pgTwoPName);
	fprintf(stream, "%s\n%s\n", pageTwo[0], pageTwo[1]);
}
