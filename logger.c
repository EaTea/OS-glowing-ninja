#include "os-project.h"

#include <time.h>

void setupLogging()
{
	logger = fopen("program.log","a");
	lf = logger != NULL;
	if(!lf)
	{
		perror("Could not open log file. No data will be logged.");
	}
}

void newLogSession()
{
	time_t rawtime;
	struct tm * ptm;

	time ( &rawtime );
	ptm = localtime ( &rawtime );
	fputs("\n===========================\n",logger);
	fputs("NEW INVOCATION SESSION\n", logger);
	fprintf(logger, "TIME: %s\n", asctime(ptm));
}

void setupMemoryDump(char* fileName)
{
	memoryDumpStream = fopen(fileName,"w");
	if(memoryDumpStream == NULL)
	{
		fputs("Error: Could not open memory dump location\n",stderr);
	}
}

void tearDownMemoryDump()
{
	fclose(memoryDumpStream);
	return;
}
