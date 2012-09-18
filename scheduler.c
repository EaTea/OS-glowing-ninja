#include "scheduler.h"

int main(int argc, char *argv[]) {
  
  //Enable logging
  *log = fopen("program.log","a"); 
  if (log == NULL) {
    perror("Could not open log file. No data will be logged"); lf = 0;
  } else lf = 1;
  if (lf) fputs(log,"===========================");
  
  progname = *argv;
  argc--; argv++; //Skip progname
  if (argc > 3 || argc < 2) {
    fputs(stderr,"ERROR: Invalid arguments\n");
    usage();
    exit(0);
  }
  
  //Algorithm Parsing
  if (!strcmp(*argv,"FCFS")) {
    alg_flag = FCFSALG;
  } else if (!strcmp(*argv,"RR")) {
    alg_flag = RRALG;
  } else {
    fputs(stderr,"ERROR: Invalid algorithm flag.");
    usage();
    exit(0); //Invalid algorithm flag
  }
  
  argv++;
  if (alg_flag == RRALG) {
    //Parse time quantum;
    if (!isint(*argv) || (time_quant = strtol(*argv,NULL,10)) == 0) {
      fputs(stderr,"ERROR: Invalid or missing time quantum for RR");
      usage(); 
      exit(0); 
    }
    if (lf) fprintf(log,"%d\n",time_quant);
    argv++;
  }
  
  //parse input program.
  
  
  if (lf) fprintf(log,"Using Algorithm: %d\n",alg_flag);
  parsefiles(*argv);
  if (lf) fprintf(log,"%d files successfully read\n",nfiles);
  return 0;
}
