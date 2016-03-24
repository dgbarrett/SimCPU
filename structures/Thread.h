#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "BurstQueue.h"
#include "BurstNode.h"
#include "../error/errors.h"

#define DEFAULT_EXIT_VALUE -1

/*the thread type is the unit that is passed around between
  various queues in the CPUSim (job, io, ready)*/

typedef struct Thread {
    int process_number;         /*represents the process to which this thread belongs*/
    int thread_number;          /*thread number w.r.t. process*/
    int arrival_time;           /*time it arrives in CPUSim */
    int start_time;             /*time when it begins execution */
    int io_time_remaining;      /*for decrementing in IO queue*/
    int cpu_time;               /*length of the current cpu burst*/
    int io_thread_total;        /*total io time done by thread*/
    int cpu_thread_total;       /*total cpu time done by thread*/
    int exit_time;              /*time it exits the CPUSim*/
    int bursts;                 /*number of cpu-io burst pairs*/
    BurstQueue * burst_queue;   /*execution stack of the thread*/
    struct Thread * next;       /*pointer to next thread, used by Queue*/
}Thread;

int          T_addBurst                 (Thread * thread, int cpu_time, int io_time);
int          T_getNumberOfBursts        (Thread * thread);
int          T_setTimings               (Thread * thread);
Thread *     T_create                   (int process_number, int thread_number, int arrival_time, int cpu_bursts);
void         T_print                    (Thread * thread);
void         T_destroy                  (Thread * thread);

#endif /* __THREAD_H__ */
