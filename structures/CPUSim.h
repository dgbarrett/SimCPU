#ifndef __CPUSIM_H__
#define __CPUSIM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Queue.h"
#include "Thread.h"
#include "BurstQueue.h"
#include "BurstNode.h"
#include "../error/errors.h"

#define NO_QUANTUM_VALUE -1
#define IO_COMPLETED 0
#define EXIT -99

/*Flag type used to represent possible user flag inputs (-d, -v) within the CPUSim structure*/
typedef enum Flag {
    UNSET = 0,  /*flags can be SET or UNSET*/
    SET = 1
} Flag;

/*used to tell a dispatcher function which queue to place an object in*/
typedef enum Destination {
    READY = 0,          /*objects can be placed in the IO, JOB, or READY queues*/
    IO = 1,
    JOB = 2
}Destination;

/*represents what the CPU is currently doing*/
typedef enum Mode {
    EXECUTING = 0,      /*CPU can be in one of 5 modes, see simcpu.c for further info*/
    DISPATCHING = 1,
    NEWCPU = 2,
    PSWITCH = 3,
    TSWITCH = 4
}Mode;


/*Holds all info about all processes, all flags, everything to do with the scheduling
  and executing of the processes*/
typedef struct CPUSim {
    Flag verbose;               /*SET if -v included in program invokation*/
    Flag detailed;              /*SET if -d included in program invokation*/
    Flag round_robin;           /*SET if -r included in program invokation*/
    int clock;                  /*the main clock for the CPU*/
    int cpu_is_executing;       /*set to 1 when the CPU is in the middle of a burst, 0 otherwise*/
    int num_of_threads;         /*total number of threads in all processes in CPU*/
    int num_of_processes;       /*number of processes being worked on by CPU*/
    int prev_process;           /*process number of previous process, uses for choosing between thread or process switch*/
    int process_switch;         /*time it takes to switch process*/
    int thread_switch;          /*time it takes to switch to different thread in same procees*/
    int time_quantum;           /*time quantum for use in RR if included*/
    int total_cpu_execution_time;   /*incremented for every CPU tick in which it is executing*/
    int wait;                   /*tells the cpu for how long to wait during context switch/burst execution before changing mode*/
    Mode mode;                  /*current mode of the CPU*/
    Thread * current_thread;    /*the thread that the CPU is currently working on*/
    struct Queue * ready_queue; /*CPU ready queue*/
    struct Queue * io_queue;    /*CPU io queue, home of blocked threads*/
    struct Queue * job_queue;   /*all threads parsed from file are initialized into job queue*/
}CPUSim;

CPUSim *      CPU_create                             ();
int           CPU_addArrivingThreadsToReadyQueue     (CPUSim * cpu);
int           CPU_addFinishedIOThreadsToReadyQueue   (CPUSim * cpu);
int           CPU_addThread                          (CPUSim * cpu, Thread * thread, Destination dest);
int           CPU_canContinue                        (CPUSim * cpu, struct Queue * exit_queue);
int           CPU_executeThreadFCFS                  (CPUSim * cpu, struct Queue * q);
int           CPU_executeThreadRR                    (CPUSim * cpu, struct Queue * q);
int           CPU_getNumberOfProcesses               (CPUSim * cpu);
int           CPU_getNumberOfThreads                 (CPUSim * cpu);
int           CPU_getNextThread                      (CPUSim * cpu);
void          CPU_advanceClock                       (CPUSim * cpu);
void          CPU_calculateStatistics                (CPUSim * cpu, struct Queue * q);
void          CPU_checkStatus                        (CPUSim * cpu);
void          CPU_executeThread                      (CPUSim * cpu, struct Queue * q);
void          CPU_setMode                            (CPUSim * cpu, Mode mode);


#endif /* __CPUSIM_H__ */
