#ifndef __BURSTQUEUE_H__
#define __BURSTQUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include "BurstNode.h"
#include "../error/errors.h"

/*the BurstQueue type is effectivley the execution stack of a thread, I called it
a queue because I am loading it in a FIFO style*/


typedef struct BurstQueue{
    struct BurstNode * head;    /*pointer to the head of the queue*/
    struct BurstNode * tail;    /*pointer to the tail of the queue*/
}BurstQueue;

BurstQueue *    BQ_create           ();
BurstNode *     BQ_getHead          (BurstQueue * bq);
BurstNode *     BQ_dequeue          (BurstQueue * bq);
int             BQ_enqueue          (BurstQueue * bq, int cpu_time, int io_time);
int             BQ_setHead          (BurstQueue * bq, int cpu_time, int io_time);
int             BQ_setTail          (BurstQueue * bq, int cpu_time, int io_time);
void            BQ_print            (BurstQueue * bq);
void            BQ_destroy          (BurstQueue * bq);

#endif /*__BURSTQUEUE_H__*/
