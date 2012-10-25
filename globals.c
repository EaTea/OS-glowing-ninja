#include "os-project.h"

char *progname;
int alg_flag = 0;
int time_quant = -1;
char **files = NULL;
int nfiles = 0;
int lf;
FILE *logger = NULL;
int timeSoFar = 0;

//cache constants
int cacheStart = -4;
char cacheProcessName[BUFSIZ];
