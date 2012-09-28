#include "queue.h"
#include <stdio.h>
#include <time.h>

int main() {
  srand( time(NULL) );
  QUEUE q = new_queue(10);
  printf("queue empty? %d\n",is_empty(q));
  PROCESS *px = malloc(sizeof(PROCESS));
  px->stime = random()%100;
  printf("Process px has start time %d\n",px->stime);
  puts("enqueuing px");
  enqueue(q,px);
  printf("front=%d,rear=%d,queue empty? %d\n",q.first,q.last,is_empty(q));
  printf("Added process with start-time %d\n",front(q)->stime);
  return 0;
}