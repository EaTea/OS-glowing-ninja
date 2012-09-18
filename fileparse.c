#include "scheduler.h"

void trimline(char *line) {
  while (*line) {
    if (*line == '\n' || *line == '\r') {
      *line = '\0';
      break;
    }
    line++;
  }
}

/**This reads each file that in.file actually contains, one by one, and stores them in a struct*/
PROCESS *readfiles() {
 fprintf(logger,"%d files\n",nfiles);
 PROCESS *processes = malloc(nfiles*sizeof(PROCESS));
 if (files == NULL) {
  perror("Cannot allocate to processes");
  exit(0);
 }
 
 FILE *fp;
 char **fparse = files;
  PROCESS *pp = processes;

 //Reading the files in one by one and storing to "processes"
 while (*fparse) {
   if ((fp = fopen(*fparse,"r")) == NULL) {
     char error[BUFSIZ];
     sprintf(error,"Cannot open %s",*files);
     perror(error);
     exit(0); //Exit if reading file fails -- MAY NOT BE THE CASE!

   } else { //Parse the file line-by-line
     char line[BUFSIZ];
     
     fgets(line,sizeof line,fp); //Read first line
       trimline(line);
       if (isint(line)) pp->stime = strtol(line,NULL,10);
  
    else {
	 fprintf(stderr,"Start time missing from %s\n",*fparse);
	 exit(0);
       }
     while (INFILE(fp)) { //Read rest of doc.
       fgets(line,sizeof line,fp);
       trimline(line);
       //check for existence of ifline
       if (tolower(line[0]) == 'i' && tolower(line[1]) == 'f') {
	 fprintf(logger,"IF LINE FOUND IN %s, line %d: \n\"%s\"\n",*fparse,pp->nlines+2,line);
	 IFLINE il = pp->iflines[pp->nifs];
	 char c;
	 sscanf(line,"if %c < %d %c = %c+1 goto %d",&(il.ifvar),&(il.max),&c,&c,&(il.gotoline));
	 fprintf(logger,"if %c < %d goto %d\n",il.ifvar,il.max,il.gotoline);
	 ++pp->nifs;
       }
       ++pp->nlines;
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
PROCESS *parsefiles(char *fname) {
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
      trimline(line);
      files[nfiles] = malloc(sizeof line);
      if (files == NULL) {
	perror("Cannot allocate to files");
	exit(0);
      }
      strcpy(files[nfiles],line);
      ++nfiles;
    }
//     realloc(files,(nfiles+1)*sizeof(char*)); //NOT SURE IF THIS LINE SHOULD BE THERE -- COULD CAUSE PROBLEMS!
  }
  fclose(fp);
  
  return readfiles();
}
