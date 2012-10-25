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
}
