#include "os-project.h"
#include <regex.h>

//Nic's awesome regular expression - We deserve bonus marks for this :)
//Matches "if i < 3 i = i+1 goto 4" with arbitrary whitespace -- Can
//Also be case-insensitive depending on how regcomp calls it.
const char *PATTERN =
"^[[:space:]]*if[[:space:]]\\+\\([[:alpha:]]\\)[[:space:]]*<[[:space:]]*\\"\
"([[:digit:]]\\+\\)[[:space:]]\\+\\1[[:space:]]*=[[:space:]]*\\1[[:space:]]*"\
"+[[:space:]]*1[[:space:]]\\+goto[[:space:]]\\+\\([[:digit:]]\\+\\)[[:space:]]*$";

void trimLine(char *line) {
	while (*line) {
		if (*line == '\n' || *line == '\r') {
			*line = '\0';
			break;
		}
		line++;
	}
}

int findIfLine(PROCESS *p, char *line, int ln) {
	regex_t regex;
	int reti;
	
	char msgbuf[100];
	int nmatches = 4;
	regmatch_t m[1024];
	
	/* Compile regular expression */
	reti = regcomp(&regex, PATTERN, REG_ICASE);
	if(reti){
		fprintf(stderr, "Could not compile regex\n"); 
		exit(1); 
	}
	/* Execute regular expression */
	reti = regexec(&regex, line, nmatches, m, 0);
	if(!reti) {
		//Found an ifline!
		++(p->nifs);
		p->iflines = (IFLINE*) realloc(p->iflines, (p->nifs)*sizeof(IFLINE));
		IFLINE* il = p->iflines + (p->nifs - 1);
		il->looped = 0;
		il->originline = ln;
		
		//loop variable
		il->ifvar = tolower(line[(int)m[1].rm_so]);
		
		char numbs[10] = "", numbs2[10] = "";
		
		//loop Limit variable
		int s = (int)m[2].rm_so, e = (int)m[2].rm_eo;
		strncpy(numbs,line+s,e-s);
		il->loopLimit = strtol(numbs,NULL,10);
		
		//goto Line
		s = (int)m[3].rm_so;
		e = (int)m[3].rm_eo;
		strncpy(numbs2,line+s,e-s);
		il->gotoline = strtol(numbs2,NULL,10);
		regfree(&regex);	
		return 1;
	} else if( reti == REG_NOMATCH ) {
		return 0;
	} else {
		//Aww balls. Somethign went horrid.
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}

/* Free compiled regular expression if you want to use the regex_t again */
	return -1;
}

int findRunningTime(PROCESS *p) {
	int rtime = p->nlines;
	for (int i = 0; i < p->nifs; i++) {
		IFLINE x = p->iflines[i];
		fprintf(logger,"Looking at ifline %d\nGo from %d to %d a total of %d times\n",i,x.originline,x.gotoline,x.loopLimit);
		rtime += (x.originline-x.gotoline+1)*x.loopLimit;
	}
	return rtime;
}

/**This reads each file that in.file actually contains, one by one, and stores them in a struct*/
PROCESS *readFiles() {
	if (lf)
		fprintf(logger,"Job has %d files\n",nfiles);
	PROCESS *processes = malloc(nfiles*sizeof(PROCESS));
	if (files == NULL) {
		perror("Cannot allocate to processes");
		exit(1);
	}

	FILE *fp;
	PROCESS *pp = processes;

	//Reading the files in one by one and storing to "processes"
	for(int fileCount = 0; fileCount < nfiles; fileCount++) {

		char **fparse = files + fileCount;
		if ((fp = fopen(*fparse,"r")) == NULL) {
			char error[BUFSIZ];
			sprintf(error,"Cannot open %s",*files);
			fprintf(logger, "Fatal Error: %s\n", error);
			perror(error);
			exit(1); //Exit if reading file fails -- MAY NOT BE THE CASE!

		} else {

			//Parse the file line-by-line
			char line[BUFSIZ];
		 
			if (fgets(line,sizeof line,fp) == NULL) {//Read first line
				perror("Cannot process file");
				exit(0);
				} else {
				trimLine(line);
				if (isint(line)) {
					pp->pname = malloc(strlen(*fparse)+1);
					strcpy(pp->pname,*fparse);
					//file is valid and has a start time at the beginning
					pp->stime = strtol(line,NULL,10);
					//construct a new process and initialise its default values
					pp->nlines = pp->nifs = pp->runningTime = 0;
					pp->curLine = 1;
					//apparently causes a memory access error if not first set to NULL
					pp->iflines = (IFLINE*) NULL;
					pp->scheduledTimeSlots = (int*) NULL;
					pp->durationTimeSlots = (int*) NULL;
					pp->nTimeSlots = 0;
					pp->lines = (char**) NULL;
				} else {
					fprintf(stderr,"Start time missing from %s\n",*fparse);
					fprintf(logger, "Fatal Error: Start time missing from %s\n",*fparse);
					exit(1);
				}
			
				while (INFILE(fp)) { //Read rest of proc.
					if (fgets(line,sizeof line,fp) != NULL) {
						trimLine(line);
						++(pp->nlines);
						pp->lines = (char**)realloc(pp->lines, pp->nlines * (sizeof(char*)));
						//explictly initialise pointer to NULL to avoid memory referential issues
						(pp->lines)[pp->nlines-1] = (char*) NULL;
						(pp->lines)[pp->nlines-1] = malloc((strlen(line) + 1) * sizeof(char));
						strcpy((pp->lines)[pp->nlines-1], line);
						//check for existence of ifline
						if (findIfLine(pp,line,pp->nlines)) {
							IFLINE *il = pp->iflines;
							fprintf(logger,"line %d: if %c < %d goto %d\n",il->originline, il->ifvar,il->loopLimit,il->gotoline);
						}
					}	
				}
			}
			pp->currtime = 0;
			pp->runningTime = findRunningTime(pp);
			if (lf) fprintf(logger,"Read file %s\n",*fparse);
			fparse++;
			pp++;
		}
		fclose(fp);
	}

	pp = processes;
	if (lf) 
		for (int i = 0; i < nfiles; i++,pp++)
			fprintf(logger,"Process %s has starttime %d and running time %d, with %d ifs\n",files[i],pp->stime,pp->runningTime,pp->nifs);
	return processes;
}

/** Parses the in.file to get the name of all the input files */
PROCESS *parseFiles(char *fname) {
	//attempt to open file
	FILE *fp;
	if ((fp = fopen(fname,"r")) == NULL) {
		char error[BUFSIZ];
		sprintf(error,"Error opening %s",fname);
		perror(error);
		usage();
		exit(0);
	} else {
		files = (char**) realloc(files,(nfiles+1)*sizeof(char*));
		//files = malloc(BUFSIZ*sizeof(char*));
		if (files == NULL) {
			perror("Cannot allocate to files");
			exit(0);
		}
		//Parsing all the filenames.
		char* strCheck;
		char line[BUFSIZ];
		while (INFILE(fp) && (strCheck = fgets(line,sizeof line,fp)) != NULL ) {
			files = realloc(files,(nfiles+1)*sizeof(char*)); //Yes, C99 prefers you use void pointers.
			//TODO: what about an empty line?
			trimLine(line);
			files[nfiles] = malloc(sizeof line);
			if (files[nfiles] == NULL) {
				perror("Cannot allocate space for file name");
				exit(0);
			}
			strcpy(files[nfiles],line);
			++nfiles;
		}
	}
	fclose(fp);
	
	return readFiles();
}
