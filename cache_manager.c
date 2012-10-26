#include "os-project.h"

int inCache(PROCESS* p, int lineNumber)
{
	//page one process is sitting inside memory
	if(!strcmp(p->pname, cacheProcessName))
		//page one's startline matches the line number
		if(cacheStart <= lineNumber && cacheStart+4 > lineNumber)
			return 1;
	return 0;
}

void loadIntoCache(PROCESS* p, int lineNumber)
{
	//record this process as the new process in cache
	strcpy(cacheProcessName, p->pname);
	//record the line number of the cache
	cacheStart = lineNumber;
	dumpCacheToStream(stdout);
}

void dumpCacheToStream(FILE* stream)
{
	int i = 0;
	while(i < nfiles && strcmp(cacheProcessName, processList[i].pname)) i++;
	fprintf(stream, "Cache: Page One\nProcess Cached: %s\n",cacheProcessName);
	fprintf(stream, "%s\n%s\n", 
			processList[i].lines[cacheStart-1],
			cacheStart+1 <= processList[i].nlines ? processList[i].lines[cacheStart] : "<<NO LINE IN CACHE>>");
	fprintf(stream, "Cache: Page Two\nProcess Cached: %s\n",cacheProcessName);
	fprintf(stream, "%s\n%s\n", 
			cacheStart+2 <= processList[i].nlines ? processList[i].lines[cacheStart+1] : "<<NO LINE IN CACHE>>",
			cacheStart+3 <= processList[i].nlines ? processList[i].lines[cacheStart+2] : "<<NO LINE IN CACHE>>");
}
