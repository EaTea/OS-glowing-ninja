#include "queue.h"
#include <stdio.h>
#include <time.h>

int main() {
  srand( time(NULL) );
  QUEUE q = new_queue(10);
  printf("queue empty? %d\n",is_empty(q));
  printf("front=%d,rear=%d,queue empty? %d\n",q.first,q.last,is_empty(q));
  for (int i = 0; i < 10; i++) {
    PROCESS *px = malloc(sizeof(PROCESS));
    printf("Adding element #%d\n",i+1);
    px->stime = random()%100;
    printf("Process px has start time %d\n",px->stime);
    //puts("enqueuing px");
    enqueue(&q,px);
    //printf("Added process with start-time %d\n",front(q)->stime);
  }
  
  while (!is_empty(q)) {
    PROCESS *py = dequeue(&q);
    printf("Front of queue has start time: %d\n",py->stime);
  }
  return 0;
}