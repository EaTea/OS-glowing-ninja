#include <stdlib.h>
#include "os-project.h"

/**Definition of a Circular Array Queue*/
typedef struct q {
  PROCESS **els;
  int first;
  int last;
  int length;
} QUEUE;

extern QUEUE new_queue(int);
extern PROCESS *front(QUEUE);
extern PROCESS *dequeue(QUEUE*);
extern void enqueue(QUEUE*, PROCESS*);
extern int is_empty(QUEUE);
extern int is_full(QUEUE);