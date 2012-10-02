#include "os-project.h"

void fcfs(PROCESS *ps) {
  int time = 0;
}

void rr(PROCESS *ps, int tq) {
  
}

int schedule(int flag,PROCESS *ps) {
  switch (flag) {
    case FCFSALG: fcfs(ps); break;
    case RRALG: rr(ps,time_quant); break;
    default: fprintf(stderr,"Error: Invalid Algorithm\n"); exit(0); break;
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
  
  char *algorithm = alg_flag == RRALG ? "Round Robin" : "First Come First Served";
  if (lf) fprintf(logger,"Using Algorithm: %s\n",algorithm);
  PROCESS* processes = parseFiles(*argv);
  if (lf) fprintf(logger,"%d files successfully read\n",nfiles);
  PROCESS* p = processes;
  if (lf) {
    for (int i = 0; i < nfiles; i++,p++) {
			fprintf(logger,"Process %d: Start time %d\n",i,p->stime);
		}
	}
 
  qsort(processes,nfiles,sizeof(PROCESS),cmpByStartTime);
  p = processes;
  if (lf) {
    fprintf(logger,"PROCESS ARRAY NOW SORTED\n");
    for (int i = 0; i < nfiles; i++,p++) 
		{
			computeProcessRunTime(p);
			fprintf(logger,"Process %d: Start time=%d and Running time=%d\n",i,p->stime, p->runningTime);
		}
  }		
  
  schedule(alg_flag,processes);
    
  fclose(logger);
  return 0;
}
