#include "scheduler.h"

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
  parsefiles(*argv);
  if (lf) fprintf(logger,"%d files successfully read\n",nfiles);
  fclose(logger);
  return 0;
}
