#include "os-project.h"

int schedule(int flag,PROCESS *ps) {
  switch (flag) {
    case FCFSALG: fcfs_algorithm(ps); break;
    case RRALG: /*rr(ps,time_quant)*/printf("We haven't implemented this shit yet, foo\n"); break;
    default: fprintf(stderr,"Error: Invalid Algorithm\n"); exit(0); break;
  }
  
  fprintf(logger,"Scheduling complete. Result of Schedule:\n");
  //TODO: Move to print_schedule fn in auxfns.c
  for (int i = 0; i < nfiles; i++) {
	printf("Process %d: ",i);
	for (int j = 0; j < ps[i].nTimeSlots; j++) {
		printf("%d\t",ps[i].scheduledTimeSlots[j]);
	} printf("\n");
  }
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
  
  schedule(alg_flag,processes);
    
  fclose(logger);
  return 0;
}
