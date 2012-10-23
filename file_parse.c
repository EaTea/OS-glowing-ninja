#include "os-project.h"

void trimLine(char *line) {
	while (*line) {
		if (*line == '\n' || *line == '\r') {
			*line = '\0';
			break;
		}
		line++;
	}
}
/*
int findRunningTime(FILE *fp) {
	//TODO: Use this function if we need to actually parse the FILE. Otherwise
	//		use the overloaded version
	return 0;
}*/

int findRunningTime(PROCESS *p) {
	int rtime = p->nlines;
	fprintf(logger,"nifs = %d",p->nifs);
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
		fprintf(logger,"%d files\n",nfiles);
	PROCESS *processes = malloc(nfiles*sizeof(PROCESS));
	if (files == NULL) {
		perror("Cannot allocate to processes");
		exit(0);
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
			exit(0); //Exit if reading file fails -- MAY NOT BE THE CASE!

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
					pp->currentLine = 1;
					//apparently causes a memory access error if not first set to NULL
					pp->iflines = (IFLINE*) NULL;
					pp->scheduledTimeSlots = (int*) NULL;
					pp->nTimeSlots = 0;
				} else {
					fprintf(stderr,"Start time missing from %s\n",*fparse);
					fprintf(logger, "Fatal Error: Start time missing from %s\n",*fparse);
					//TODO: should this be exit(0)? or exit(1)?
					exit(0);
				}
			
				while (INFILE(fp)) { //Read rest of doc.
					if (fgets(line,sizeof line,fp) != NULL) {
						trimLine(line);
						++(pp->nlines);
						//check for existence of ifline
						if (tolower(line[0]) == 'i' && tolower(line[1]) == 'f') {
							//found a new IFLINE
							fprintf(logger,"IF LINE FOUND IN %s, line %d: \n\"%s\"\n",*fparse,pp->nlines+2,line);
							//increase number of iflines recorded
							++(pp->nifs);
							//increase size of IFLINE array by 1
							pp->iflines = (IFLINE*) realloc(pp->iflines, (pp->nifs)*sizeof(IFLINE));
							IFLINE* il = pp->iflines + (pp->nifs - 1);
							//the line that this ifline is on
							il->originline = pp->nlines;
							il->looped = 0;
							//sentinel character
							char c;
							//read using sscanf
							sscanf(line,"if %c < %d %c = %c+1 goto %d",&(il->ifvar),&(il->loopLimit),&c,&c,&(il->gotoline));
							//TODO: If string is poorly formatted, might require regex instead of sscanf.
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
		//realloc(files,(nfiles+1)*sizeof(char*)); //NOT SURE IF THIS LINE SHOULD BE THERE -- COULD CAUSE PROBLEMS!
	}
	fclose(fp);
	
	return readFiles();
}
