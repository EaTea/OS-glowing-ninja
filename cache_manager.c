#include "os-project.h"

static const char* NO_CACHE_VALUE = "<<NO LINE STORED>>";

static char **pageOne;
static char **pageTwo;

//cache constants
static int pgOneStart;
static char pgOnePName[BUFSIZ];
static int pgTwoStart;
static char pgTwoPName[BUFSIZ];

void initialiseCache()
{
	printf("Initialising cache\n");
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
		if(pgOneStart <= lineNumber && pgOneStart+1 > lineNumber)
			return 1;
	//page two process is sitting inside memory
	if(!strcmp(p->pname, pgTwoPName))
		//page two's startline matches the line number
		if(pgTwoStart <= lineNumber && pgTwoStart+1 > lineNumber)
			return 1;
	return 0;
}

void loadIntoCache(PROCESS* p, int lineNumber)
{
	if(p == NULL)
	{
		//throw an error
		puts("Tried to load a NULL pointer into cache\n");
		return;
	}
	if(lineNumber > p->nlines)
	{
		//throw an error
		puts("Tried to load a block entirely outside of process into cache\n");
		return;
	}
	//record this process as the new process in cache
	strcpy(pgOnePName, p->pname);
	strcpy(pgTwoPName, p->pname);
	//record the line number of the cache
	pgOneStart = lineNumber;
	pgTwoStart = lineNumber+2;
	strcpy(pageOne[0],p->lines[lineNumber-1]);
	//hit the end of process whilst attempting to load process into cache
	//record "INVALID LINE" if this occurs
	strcpy(pageOne[1], lineNumber+1 <= p->nlines ? p->lines[lineNumber] : NO_CACHE_VALUE);
	strcpy(pageTwo[0], lineNumber+2 <= p->nlines ? p->lines[lineNumber+1] : NO_CACHE_VALUE);
	strcpy(pageTwo[1], lineNumber+3 <= p->nlines ? p->lines[lineNumber+2] : NO_CACHE_VALUE);
}

void dumpCacheToStream(FILE* stream)
{
	fprintf(stream, "Cache: Page One\nProcess Cached: %s\n",pgOnePName);
	fprintf(stream, "%s\n%s\n", pageOne[0], pageOne[1]);
	fprintf(stream, "Cache: Page Two\nProcess Cached: %s\n",pgTwoPName);
	fprintf(stream, "%s\n%s\n", pageTwo[0], pageTwo[1]);
}
