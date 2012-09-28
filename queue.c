#include "queue.h"

QUEUE new_queue(int n) {
  QUEUE *q = malloc(sizeof(QUEUE));
  q->els = malloc((n+1)*sizeof(PROCESS*));
  //q[q->first] = NULL;
  q->length = n;
  q->first = 0;
  q->last = -1;
  return *q;
}

int is_empty(QUEUE q) {
  return q.first == (q.last+1)%q.length;
}

int is_full(QUEUE q) {
  return q.first == (q.last+2)%q.length;
}

PROCESS *front(QUEUE q) {
  if (!is_empty(q)) return q.els[q.first];
  else return NULL;
}

PROCESS *dequeue(QUEUE *q) {
  if (!is_empty(*q)) {
    PROCESS *p = (q->els)[q->first];
    q->first = (q->first+1)%q->length;
    return p;
  } else return NULL;
}

void enqueue(QUEUE *q, PROCESS *p) {
  if (!is_full(*q)) {
    q->last = (q->last+1)%q->length;
    printf("ENQUEUING: First = %d, Last = %d\n",q->first,q->last);
    q->els[q->last] = p;
  } else puts("Array full!!!");
}

