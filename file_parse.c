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
	if (lf) fprintf(logger,"%d files\n",nfiles);
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
			perror(error);
			exit(0); //Exit if reading file fails -- MAY NOT BE THE CASE!
		}
		
		//Parse the file line-by-line
		char line[BUFSIZ];
		
		if (fgets(line,sizeof line,fp) == NULL) {//Read first line
			perror("Cannot process file");
			exit(0);
		} else {
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
				exit(1);
			}
			
			while (INFILE(fp)) { //Read rest of doc.
				if (fgets(line,sizeof line,fp) == NULL) {
					perror("Cannot process file");
					exit(0);
				}
				trimLine(line);
				//check for existence of ifline
				if (tolower(line[0]) == 'i' && tolower(line[1]) == 'f') {
					if (lf) fprintf(logger,"IF LINE FOUND IN %s, line %d: \n\"%s\"\n",*fparse,pp->nlines+2,line);
					IFLINE il = pp->iflines[pp->nifs];
					char c;
					sscanf(line,"if %c < %d %c = %c+1 goto %d",&(il.ifvar),&(il.looped),&c,&c,&(il.gotoline));
					if (lf) fprintf(logger,"if %c < %d goto %d\n",il.ifvar,il.looped,il.gotoline);
					++pp->nifs;
				}
				++pp->nlines;
			}
		}
		
		if (lf) fprintf(logger,"Read file %s\n",*fparse);
		fparse++;
		pp++;
	}
	fclose(fp);

	pp = processes;
	
	if (lf) 
		for (int i = 0; i < nfiles; i++,pp++)
			fprintf(logger,"Process %s has starttime %d and %d lines, with %d ifs\n",files[i],pp->stime,pp->nlines,pp->nifs);
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
			files = (char*) realloc(files,(nfiles+1)*sizeof(char*));
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
