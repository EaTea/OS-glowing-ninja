#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FCFSALG 1
#define RRALG 2

#define INFILE(a) !(feof(a) || ferror(a))

typedef struct {
    char ifvar; //The character for the variable
    int gotoline;
    int looped;
    int max;
} IFLINE;

typedef struct {
  int curline;
  int stime;
  int nlines;
  int nifs;
	int runningTime;
  IFLINE iflines[BUFSIZ];
	int nTimeSlots;
	int* scheduledTimeSlots;
} PROCESS;

extern char *progname;

/*
 * Prints program usage message to standard output stream.
 */
extern void usage();

/*
 * Returns whether a given char* is an integer; that is, does the given char*
 * contain only digits.
 * Returns zero if the char* parameter contains a non-digit character. Thus,
 * isint("5.0"), isint("-5") and isint("5a") will return 0.
 * Returns a nonzero value otherwise.
 */
extern int isint(char*);

/*
 * Comparator to determine if process a starts earlier than process b.
 * This comparator sorts by start time, and returns
 *  - a negative number if a starts before b
 *  - zero if a and b start at the same time
 *  - a positive number if a starts after b
 */
extern int cmpByStartTime(const void*, const void*);

/*
 * Returns the maximum of two integers
 */
extern int max(int, int);

/*
 * Returns the minimum of two integers
 */
extern int min(int, int);

extern char **files;

/*
 * Parses each file contained within the jobs file and stores each as a process structure.
 * Returns a pointer to all the process structures that were constructed by parseFiles
 */
extern PROCESS * parseFiles(char*);

extern int alg_flag;
extern int time_quant;
extern int nfiles;

extern FILE *logger;
extern int lf;

/*
 * Opens the logging stream and sets the log flag, otherwise prints an error.
 * Note: setupLogging() does not cause program failure; inability to log is considered
 * a non-fatal error.
 */
extern void setupLogging();

/*
 * Prints a new logging session to the log file.
 */
extern void newLogSession();
