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

	//actual lines stored as strings
	char **lines;
} PROCESS;

/**
	Frame definition for Main Memory
	Also serves as a Doubly Linked List element
*/
typedef struct frame {
	//name of the process
	char pname[BUFSIZ];
	//the first line that the frame holds, indexed from 1
	//a frame holds lineStart and (if it exists) lineStart+1
	//lineStart is guaranteed to exist in process named by pname
	int lineStart;
	//the actual lines in the page stored as strings
	char **page;
	//the next frame in the linked list; should be NULL iff this is the last element
	//in the linked list
	struct frame* next;
	//the previous element in the linked list; should be NULL iff this is the last
	//element in the linked list
	struct frame* previous;
} FRAME;
/*
	Safely constructs a new frame that is guaranteed to be safely insertable into a new linked list
	returns a new frame that has been dynamically allocated
	*/
FRAME* newFrame();
/*
	Recursively destroy a FRAME
	Accepts the memory location of the FRAME to destroy
	Will then proceed to destroy its successor until the entire list has been destroyed
	 */
void recursiveDestroyFrame(FRAME*);


extern char *progname; //Program name - used in errors



/*
 * Prints program usage message to standard output stream.
 */
extern void usage();

/*
 * Prints computed schedule to standard output stream
 */
extern void print_schedule(PROCESS*); 


/*
 * Returns whether a given char* is an integer; that is, does the given char*
 * contain only digits.
 * Returns zero if the char* parameter contains a non-digit character. Thus,
 * isint("5.0"), isint("-5") and isint("5a") will return 0.
 * Returns a nonzero value otherwise.
 */
extern int isint(char*);

/*
	Simulates the First-Come-First-Serve scheduling algorithm on an array of processes
	Accepts an array of PROCESSes that has been sorted by starting time

	 */
extern void fcfs_algorithm(PROCESS*);

/*
 * Simulates a Round-Robin scheduling on an array of PROCESSes.
 * Accepts a array of PROCESSes that has been sorted by starting time and an integer time quantum.
 * N.B.: as the time quantum approaches 0, interesting behaviour can occur.
 * N.B.: Time quantum of 0 is not allowed
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

/**
	Flag to signify which algorithm should be used for the run of the program
	*/
extern int alg_flag;
/*
	The time quantum to run the program; only used in Round-Robin Scheduling
	 */
extern int time_quant;
/*
	 Number of files to process; essentially, number of PROCESSes
	 */
extern int nfiles;
/*
	Run time of simulation so far
	 */
extern int timeSoFar;
/*
	Memory management flag; is 1 if memory management is desired for this invocation
	of the program
	 */
extern int memoryManage;
/*
	 Sentinel value that states that no value was recorded in this line of the page
	 */
extern const char* NO_VALUE;

/*
	Times when dumps should be outputted to the memory dump output stream
	*/
extern int* timesToTakeDumps;
/*
	Number of dumps to take
	*/
extern int nToDumps;
/*
	The index of the next time a dump should occur
	*/
extern int nextTimeToDumpIndex;

/*
	A file stream where all logging output should be sent to
	*/
extern FILE *logger;
/*
	An output stream where all memory dumps should be sent to
	*/
extern FILE *memoryDumpStream;
/*
	Logging flag; true iff logging can occur
	*/
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
 * Accepts the PROCESS location inside memory and line number (indexed from 1), and the FRAME location which (will) contain the line
 * Returns a nonzero value iff the line number passed has been loaded into the main memory and through pass-by-reference the FRAME which contains this line
 * N.B.: A frame contains a line l iff the first line of the frame's page is l, and the second line of the frame's page is l+1
 */
extern int inMainMemory(PROCESS*, int, FRAME**);
/*
 * Loads the line of a PROCESS, and the 3 subsequent lines into cache
 * Accepts the PROCESS to load, and the line number to load
 */
extern void loadIntoCache(FRAME*,FRAME*);

/*
	 Dumps the cache to a stream specified by a FILE*
	 Accepts the stream to write to; assumes without checking that thisstream can be written to
	*/
extern void dumpCacheToStream(FILE*);
/*
	 Dumps the main memory contents to a stream specified by a FILE*
	 Accepts the stream to write to; assumes without checking that thisstream can be written to
	*/
extern void dumpMainMemoryToStream(FILE*);

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
extern void setupMemoryDump(char*);

/*
	 Closes the memory dump stream
	*/
extern void tearDownMemoryDump();

/*
	Initialises the main memory
	*/
extern void initialiseMainMemory();

/*
	Closes the main memory linked list
	*/
extern void tearDownMainMemory();

/*
	Load into a main memory a process at a line
	Accepts a PROCESS* and a line number
	Loads that line and the line after it in the process to the main memory
	 */
extern void loadIntoMainMemory(PROCESS*,int);

/*
	Accepts a FRAME and updates the main memory by putting the frame at the
	beginning of the linked list representing the main memory
	 */
extern void updateMainMemory(FRAME*);



/**QUEUE**/
/**Definition of a Circular Array Queue*/
typedef struct q {
	//elements of the queue; these are processes
  PROCESS **els;
	//first element of the queue (index)
  int first;
	//last element of the queue (index)
  int last;
	
	//N.B.: we do not keep the length of the queue we can check if the queue is full iff
	//(last+2)%length == first

	//length of the array
  int length;
} QUEUE;

/*
	Construct a new Circular Array Queue
	Accepts the size of the array to construct
	Please don't use a size 0 or less
	*/
extern QUEUE* new_queue(int);
/*
	return the process at the front of the queue specified by QUEUE
	Accepts a pointer to a QUEUE
	Returns a pointer to the process that was at the front of the queue
	*/
extern PROCESS *front(QUEUE*);
/*
	return the process at the front of the queue specified by QUEUE
	This function will remove the first element of the QUEUE
	Accepts a pointer to a QUEUE
	Returns a pointer to the process at the front of the queue
	*/
extern PROCESS *dequeue(QUEUE*);
/*
	Enqueue an element onto the queue
	Accepts a pointer to a QUEUE and a pointer to the PROCESS to enqueue
	The PROCESS* is the element to load ontot he queue
	 */
extern void enqueue(QUEUE*, PROCESS*);
/*
	Returns 1 if the queue data structure specified by the QUEUE* is
	empty, and 0 otherwise
	 */
extern int is_empty(QUEUE*);
/*
	Returns 1 if the queue data structure specified by the QUEUE* is
	full, and 0 otherwise
	 */
extern int is_full(QUEUE*);

/**LIST**/
/**Definition of a Doubly linked list*/
typedef struct l
{
	//first element of the list, is NULL if empty
	FRAME* first;
	//last element of the list, is NULL if empty
	FRAME* last;
	//the size ofthe list, that is, the number of elements inside it
	int size;
} FRAME_LIST;

/*
	Constructs a new linked list and initialises its pointers to NULL
	 */
FRAME_LIST* newList();
/*
	Appends an element to the END of the list
	Frame list is a non-NULL element of the queue
	FRAME is a non-NULL element of the list
	This FRAME is considered to be "well-formed"
	Its first and last elements have not been set, as
	it will be the case that they may be reset/changed during the
	operation of the LIST
	 */
void appendToList(FRAME_LIST*,FRAME*);
/*
	Does a linear scan to see whether a frame containing
	a page that records the line and process exists
	FRAME_LIST is the list to scan
	char* is the process name
	int is the line number to look for
	FRAME** the address of the FRAME which matches our criterion

	Note that a frame f is considered to contain it iff
		f.lineStart == line
		strcmp(f.pname,pname) == 0
	*/
int isInList(FRAME_LIST*,char*,int,FRAME**);
/*
	Bring an element (FRAME*) to the front of a FRAME_LIST*
	Accepts the FRAME_LIST to modify
	Accepts the FRAME to bring to the front of the list
	Does nothing if the FRAME is the front if the list already
	 */
void bringElementToFront(FRAME_LIST*,FRAME*);
/*
	Destroys a list in a recursive fashion
	Accepts a FRAME_LIST* to destroy
	 */
void destroyList(FRAME_LIST*);
