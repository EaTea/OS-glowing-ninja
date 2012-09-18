#include "scheduler.h"

/** A number of Auxiliary Functions used in multiple files, such as reading ints and running the "usage" **/


void usage() {
  printf("Usage: \'%s FCFS in.file\' or \'%s RR n in.file\n",progname,progname);
  puts("where in.file is a list of job files, and n is an integer representing a time quantum");
}

int isint(char *str) {
  while (*str) {
    if (!isdigit(*str)) return 0;
    else str++;
  } return 1;
}