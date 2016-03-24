#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CPUSim.h"
#include "Thread.h"
#include "BurstQueue.h"
#include "BurstNode.h"
#include "../error/errors.h"

/*queues are used to control the flows of threads through the simulation*/

typedef struct Queue {
    Thread * head;      /*pointer to the head of the queue*/
    Thread * tail;      /*pointer to the tail of the queue*/
    int size;           /*size of the queue*/
}Queue;

int             Q_addThread             (Queue * q, Thread * thread);
Queue *         Q_create                ();
Thread *        Q_removeThread          (Queue * q);
Thread *        Q_getHead               (Queue * q);
Thread *        Q_removeThreadAtTime    (Queue * q, int time);
Thread *        Q_removeIOThreadAtTime  (Queue *q, int io_time_finished);
void            Q_decrementAllIO        (Queue * q);
void            Q_setHead               (Queue * q, Thread * thread);
void            Q_setTail               (Queue * q, Thread * thread);
void            Q_print                 (Queue * q);



#endif /* __QUEUE_H__ */
