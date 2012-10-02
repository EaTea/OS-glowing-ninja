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

/**This reads each file that in.file actually contains, one by one, and stores them in a struct*/
PROCESS *readFiles() {
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
	//while (*fparse) {
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
		 
			fgets(line,sizeof line,fp); //Read first line
			trimLine(line);
			if (isint(line))
			{
				//file is valid and has a start time at the beginning
				pp->stime = strtol(line,NULL,10);
				//construct a new process and initialise its default values
				pp->nlines = pp->nifs = pp->runningTime = 0;
				//apparently causes a memory access error if not first set to NULL
				pp->iflines = (IFLINE*) NULL;
				pp->scheduledTimeSlots = (int*) NULL;
				pp->nTimeSlots = 0;
			}
			else {
				fprintf(stderr,"Start time missing from %s\n",*fparse);
				fprintf(logger, "Fatal Error: Start time missing from %s\n",*fparse);
				//TODO: should this be exit(0)? or exit(1)?
				exit(0);
			}
			while (INFILE(fp)) { //Read rest of doc.
				fgets(line,sizeof line,fp);
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
					fprintf(logger,"line %d: if %c < %d goto %d\n",il->originline, il->ifvar,il->loopLimit,il->gotoline);
				}
			}
		}
		fprintf(logger,"Read file %s\n",*fparse);
		fparse++;
		pp++;
	}
	fclose(fp);

	pp = processes;
	for (int i = 0; i < nfiles; i++,pp++) {
		fprintf(logger,"Process %s has starttime %d and %d lines, with %d ifs\n",files[i],pp->stime,pp->nlines,pp->nifs);
	}
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
		files = malloc(BUFSIZ*sizeof(char*));
		if (files == NULL) {
			perror("Cannot allocate to files");
			exit(0);
		}
		//Parsing all the filenames.
		char line[BUFSIZ];
		while (INFILE(fp)) {
			fgets(line,sizeof line,fp);
			//TODO: what about an empty line?
			trimLine(line);
			files[nfiles] = malloc(sizeof line);
			if (files == NULL) {
				perror("Cannot allocate to files");
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
