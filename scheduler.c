#include "scheduler.h"

int main(int argc, char *argv[]) {
  
  //Enable logging
  logger = fopen("program.log","a"); 
  if (logger == NULL) {
    perror("Could not open log file. No data will be logged"); lf = 0;
  } else lf = 1;
  if (lf) fputs("\n===========================\n",logger);
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
  
  
  if (lf) fprintf(logger,"Using Algorithm: %d\n",alg_flag);
  parsefiles(*argv);
  if (lf) fprintf(logger,"%d files successfully read\n",nfiles);
  fclose(logger);
  return 0;
}
