#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

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
	char msgbuf[100];

/* Compile regular expression */
	reti = regcomp(&regex, PATTERN, REG_ICASE);
	if( reti ){ fprintf(stderr, "Could not compile regex\n"); exit(1); }

/* Execute regular expression */
	reti = regexec(&regex, "if  I < 5 i=i+1 goto 100", 0, NULL, 0);
	if( !reti ){
		puts("Match");
		
	}
	else if( reti == REG_NOMATCH ){
		puts("No match");
	}
	else{
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}

/* Free compiled regular expression if you want to use the regex_t again */
    regfree(&regex);	
}