#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "../os-project.h"

//Nic's awesome regular expression - We deserve bonus marks for this
//Matches "if i < 3 i = i+1 goto 4" with arbitrary whitespace -- Can
//Also be case-insensitive depending on how regcomp calls it.
const char *PATTERN =
	"^[[:space:]]*if[[:space:]]\\+\\([[:alpha:]]\\)[[:space:]]*<[[:space:]]*\\"\
	"(\[[:digit:]]\\+\\)[[:space:]]\\+\\1[[:space:]]*=[[:space:]]*\\1[[:space:]]*"\
	"+[[:space:]]*1[[:space:]]\\+goto[[:space:]]\\+\\([[:digit:]]\\+\\)[[:space:]]*$";
	
int main() {
	regex_t regex;
	int reti;
	char *line = "if i < 10 i=i+1 goto 4";
	puts (line);
	char msgbuf[100];
	int nmatches = 1024;
	regmatch_t m[1024];
	/* Compile regular expression */
	reti = regcomp(&regex, PATTERN, REG_ICASE);
	if(reti){
		fprintf(stderr, "Could not compile regex\n"); 
		exit(1); 
	}
	/* Execute regular expression */
	reti = regexec(&regex, line, nmatches, m, 0);
	if(!reti){
		//Found an ifline!
		++(p->nifs);
		p->iflines = (IFLINE*) realloc(p->iflines, (p->nifs)*sizeof(IFLINE));
		IFLINE* il = p->iflines + (p->nifs - 1);
// 		IFLINE *il = malloc(sizeof(IFLINE));
		
		//loop variable
		il->ifvar = tolower(line[(int)m[1].rm_so]);
		
		char numbs[10], numbs2[10];
		
		//loop Limit variable
		int s = (int)m[2].rm_so, e = (int)m[2].rm_eo;
		strncpy(numbs,line+s,e-s);
		il->loopLimit = strtol(numbs,NULL,10);
		
		//goto Line
		s = (int)m[3].rm_so;
		e = (int)m[3].rm_eo;
		strncpy(numbs2,line+s,e-s);
		il->gotoline = strtol(numbs2,NULL,10);
	} else if( reti == REG_NOMATCH ) {
		//Not an ifline -- Normal!
	} else {
		//Aww balls. Somethign went horrid.
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}

/* Free compiled regular expression if you want to use the regex_t again */
    regfree(&regex);	
}