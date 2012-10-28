#include "os-project.h"

int schedule(int flag,PROCESS *ps) {
	//if(memoryManage)
	//{
		initialiseMainMemory();
		initialiseCache();
	//}
  switch (flag) {
    case FCFSALG: fcfs_algorithm(ps); break;
    case RRALG: rr_algorithm(ps,time_quant); break;
    default: fprintf(stderr,"Error: Invalid Algorithm\n"); exit(0); break;
  }
  
  fprintf(logger,"Scheduling complete. Result of Schedule:\n");
  //TODO: Move to print_schedule fn in auxfns.c
  for (int y = 0; y < nfiles; y++) {
		//HEEEEY SEXY LADY
	printf("%s:\t",ps[y].pname);
	//OP OP
	for (int j = 0; j < ps[y].nTimeSlots; j++) {
		//OPPA GANGNAM STYLE!
		printf("(%d, %d)\t",ps[y].scheduledTimeSlots[j], ps[y].durationTimeSlots[j]);
	} printf("\n");
	//OP OP
	for(int j = 0; j < ps[y].nlines; j++)
	{
		//OPPA GANGNAM STYLE!
		printf("%s\n", ps[y].lines[j]);
  }
	}

	//if(memoryManage)
	//{
		tearDownCache();
		tearDownMainMemory();
	//}
	
  return 0;
}

int main(int argc, char *argv[]) {
  
  //Enable logging
  logger = fopen("program.log","a"); 
	setupLogging();
	newLogSession();
  progname = *argv;
  argc--; argv++; //Skip progname
  if (argc > 3 || argc < 2) {
    fputs("ERROR: Invalid arguments\n",stderr);
    usage();
    exit(0);
  }
  
  //Algorithm Parsing
  if (!strcmp(*argv,"FCFS")) {
    alg_flag = FCFSALG;
  } else if (!strcmp(*argv,"RR")) {
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
    if (lf) fprintf(stderr,"%d\n",time_quant);
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
  
  //Order by start time and number.
  qsort(processes,nfiles,sizeof(PROCESS),cmpByStartTime); 
  p = processes;
  for (int i = 0; i < nfiles; i++,p++)
    p->num = i+1;
  
  
  //TODO: change
	memoryDumpStream = fopen("memDump.out","w");
  schedule(alg_flag,processes);
    
  fclose(logger);
  return 0;
}
