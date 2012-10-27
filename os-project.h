#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FCFSALG 1
#define RRALG 2

#define INFILE(a) !(feof(a) || ferror(a))

typedef struct {
	//The character for the variable
	char ifvar;
	//line to go to
	int gotoline;
	//line to go from?
	int originline;
	//number of times looped
	int looped;
	//k --- the value ifvar must be less than for the if loop to continue
	int loopLimit;
} IFLINE;

typedef struct {
	char *pname;
	//current line -- the line about to be processed
	int curLine;
	//number (in sorted order)
	int num;
	//start time
	int stime;
	//number of lines
	int nlines;
	//number of ifs
	int nifs;
	//length of time process requires to run
	int runningTime;
	//The current time of the running process (number of cycles stepped)
	int currtime;
	//each IFLINE
	IFLINE* iflines;
	//number of time slots allocated for a process
	int nTimeSlots;
	//each of the time slots scheduled for serving a process
	int* scheduledTimeSlots;
	//duration of each time slot
	int* durationTimeSlots;

	//actual lines
	char **lines;
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

extern void fcfs_algorithm(PROCESS*);

/*
 * Simulates a Round-Robin scheduling on an array of PROCESSes.
 * Accepts a array of PROCESSes that has been sorted by starting time and an integer time quantum.
 * N.B.: as the time quantum approaches 0, interesting behaviour can occur.
 */
extern void rr_algorithm(PROCESS*,int);

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
extern int timeSoFar;
extern int memoryManage;
extern PROCESS* processList;

extern FILE *logger;
extern FILE *memoryDumpStream;
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

/*
 * TODO: I believe this will end up becoming deprecated.
 * Computes the running time of a single PROCESS data structure
 */
extern void computeProcessRunTime(PROCESS*);

/*
 * Processes a single line of a process
 * Accepts the location of the process in memory and the line count that it is up to at the moment, and the number of if lines, and the amount of time remaining
 * If this amount is less than 0, there is no limit; otherwise, the time consumed is always less than the time remaining
 * Returns the amount of time consumed to process that line
 */
extern int processLine(PROCESS*, int*, int, int);

/*
 * Run a process with a given timeslice
 * Accepts the PROCESS to be run and the maximum amount of time that this program should run for.
 * If the integer parameter is -1, then the PROCESS will run until completion.
 * This function will modify the underlying process and add the time it has consumed into the slot for this process
 * Returns the amount of time consumed in running that process.
 */
extern int runProcessInTimeSlice(PROCESS*, int);

/*
 * Checks if a line is inside the cache
 * Accepts the PROCESS memory location and line number (indexed from 1)
 * Returns a nonzero value iff the line number passed has been loaded into the cache
 */
extern int inCache(PROCESS*, int);
/*
 * Checks if a line is inside the main memory 
 * Accepts the PROCESS location inside memory and line number (indexed from 1)
 * Returns a nonzero value iff the line number passed has been loaded into the main memory
 */
extern int inMainMemory(PROCESS*, int);
/*
 * Loads the line of a PROCESS, and the 3 subsequent lines into cache
 * Accepts the PROCESS to load, and the line number to load
 */
extern void loadIntoCache(PROCESS*, int);

/*
	 Dumps the cache to a stream specified by a FILE*
	 Accepts the stream to write to; assumes without checking that thisstream can be written to
	*/
extern void dumpCacheToStream(FILE*);

/*
	 Initialise the cache using dynamic memory allocation
	 Call at the beginning of the program invocation or the cache simulation is not guaranteed to work as expected
	*/
extern void initialiseCache();
/*
	 Free any dynamic memory allocated to the cache
	*/
extern void tearDownCache();

/*
	 Sets up the memory dump stream
	*/
extern void setupMemoryDump();

/*
	 Closes the memory dump stream
	*/
extern void tearDownMemoryDump;



/**QUEUE**/
/**Definition of a Circular Array Queue*/
typedef struct q {
  PROCESS **els;
  int first;
  int last;
  int length;
} QUEUE;

extern QUEUE new_queue(int);
extern PROCESS *front(QUEUE);
extern PROCESS *dequeue(QUEUE*);
extern void enqueue(QUEUE*, PROCESS*);
extern int is_empty(QUEUE);
extern int is_full(QUEUE);
