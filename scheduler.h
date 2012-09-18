#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FCFSALG 1
#define RRALG 2

#define INFILE(a) !(feof(a) || ferror(a))

typedef struct {
    int ifvar; //The character for the variable
    int gotoline;
    int looped;
} IFLINE;

typedef struct {
  int curline;
  int stime;
  int nlines;
  int nifs;
  IFLINE iflines[BUFSIZ];
} PROCESS;



extern char *progname;

extern void usage();
extern int isint(char*);

extern char **files;
extern PROCESS *parsefiles(char*);
extern int alg_flag;
extern int time_quant;
extern int nfiles;
extern FILE *logger;
extern int lf;