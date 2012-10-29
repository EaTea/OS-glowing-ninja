#include "os-project.h"
#include <strings.h>

int schedule(int flag,PROCESS *ps) {
	//if memory management requested, make the cache
	if(memoryManage)
	{
		//initialise main memory and cache
		initialiseMainMemory();
		initialiseCache();
	}
	//depending on which algorithm, do awesomeness
  switch (flag) {
    case FCFSALG: fcfs_algorithm(ps); break;
    case RRALG: rr_algorithm(ps,time_quant); break;
    default: fprintf(stderr,"Error: Invalid Algorithm\n"); exit(1); break;
  }
  
  if (lf) fprintf(logger,"Scheduling complete.\n");
  print_schedule(ps);
	clearProcesses(ps,nfiles);

	//if memory management requested, destroy the cache
	if(memoryManage)
	{
		tearDownCache();
		tearDownMainMemory();
	}
	
  return 0;
}

//static comparator for comparing integers
static int cmpInts(const void* a1, const void* b1)
{
	//see qsort's man pages for more information
	int* a = (int*) a1;
	int* b = (int*) b1;
	return *a - *b;
}

int main(int argc, char *argv[]) {
  
  //Enable logging
  logger = fopen("program.log","a"); 
	setupLogging();
	newLogSession();
  progname = *argv;
  argc--; argv++; //Skip progname
  if (argc < 2) {
    fputs("ERROR: Invalid arguments\n",stderr);
    usage();
    exit(0);
  }
	
	if(!strcmp(*argv,"-m"))
	{
		memoryManage = 1;
		nToDumps = 0;
		timesToTakeDumps = NULL;
		argv++;
		while(isint(*argv))
		{
			nToDumps++;
			timesToTakeDumps = realloc(timesToTakeDumps,nToDumps * sizeof(int));
			timesToTakeDumps[nToDumps-1] = atoi(*argv);
			argv++;
		}
		qsort(timesToTakeDumps,nToDumps,sizeof(int),cmpInts);
		if(nToDumps)
			nextTimeToDumpIndex = 0;
	}
  
  //Algorithm Parsing
  if (!strcasecmp(*argv,"FCFS")) {
    alg_flag = FCFSALG;
  } else if (!strcasecmp(*argv,"RR")) {
    alg_flag = RRALG;
  } else {
    fputs("ERROR: Invalid algorithm flag.",stderr);
    usage();
    exit(0); //Invalid algorithm flag
  }
  
  argv++;
  if (alg_flag == RRALG) {
    //Parse time quantum;
    if (!isint(*argv) || (time_quant = strtol(*argv,NULL,10)) == 0) {
      fputs("ERROR: Invalid or missing time quantum for RR",stderr);
      usage(); 
      exit(0); 
    }
    if (lf) fprintf(logger,"Time Quantum: %d\n",time_quant);
    argv++;
  }
  
  //parse input program.
  if (lf) {
    char *algorithm = alg_flag == RRALG ? "Round Robin" : "First Come First Served";
    fprintf(logger,"Using Algorithm: %s\n",algorithm);
  }
  PROCESS* processes = parseFiles(*argv);
  if (lf) fprintf(logger,"%d files successfully read\n",nfiles);
  PROCESS* p = processes;
  if (lf) {
    for (int i = 0; i < nfiles; i++,p++)
      fprintf(logger,"Process %d: Start time %d\n",i,p->stime);
  }
	argv++;
  
  //Order by start time and number.
  qsort(processes,nfiles,sizeof(PROCESS),cmpByStartTime); 
  p = processes;
  for (int i = 0; i < nfiles; i++,p++)
    p->num = i+1;
  
	if(memoryManage)
	{
		setupMemoryDump(*argv);
	}
  schedule(alg_flag,processes);
    
  fclose(logger);
	if(memoryManage)
	{
		tearDownMemoryDump();
	}
  return 0;
}
