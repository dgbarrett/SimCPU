#include "CPUSim.h"

/*CPUSim type holds all info about all processes, all flags, everything to do with the scheduling
  and executing of the processes*/

/*allocates and initialized a new CPUSim*/
CPUSim * CPU_create()
{
    CPUSim * new_cpu = malloc(sizeof(CPUSim));

    if (!new_cpu)
    {
        MEMORY_ALLOCATION_ERROR("CPUSim *", "CPU_create", "CPUSim.c");
        return NULL;
    }

    new_cpu -> clock = 0;

    new_cpu -> mode = NEWCPU;

    new_cpu -> prev_process = -1;
    new_cpu -> wait = 0;
    new_cpu -> cpu_is_executing = 0;

    new_cpu -> detailed = UNSET;
    new_cpu -> verbose = UNSET;
    new_cpu -> round_robin = UNSET;
    new_cpu -> time_quantum = NO_QUANTUM_VALUE;

    new_cpu -> ready_queue = Q_create();
    new_cpu -> io_queue = Q_create();
    new_cpu -> job_queue = Q_create();

    new_cpu -> total_cpu_execution_time = 0;
    new_cpu -> thread_switch = -1;
    new_cpu -> process_switch = -1;
    new_cpu -> num_of_threads = -1;
    new_cpu -> num_of_processes = -1;

    return new_cpu;
}

/*moves threads that have completed their IO time from the IO (blocked)
  queue, to the ready queue*/
int CPU_addFinishedIOThreadsToReadyQueue(CPUSim * cpu)
{
    Thread * arriving_thread = NULL;

    if (!cpu)
    {
        return -1;
    }

    do
    {
        /*removes a thread from IO queue which has no IO time left*/
        arriving_thread = Q_removeIOThreadAtTime(cpu -> io_queue, IO_COMPLETED);

        /*if thread is NULL, that means there are none with that time, we skip this if*/
        if (arriving_thread != NULL)
        {
            if (cpu -> verbose == SET)
            {
                /*if in verbose mode, print verbose description*/
                printf("At Time %d: Thread %d of Process %d moves from BLOCKED to READY\n", cpu -> clock, arriving_thread -> thread_number, arriving_thread -> process_number);
            }

            /*take thread we removed from IO queue and add to Ready queue*/
            CPU_addThread(cpu, arriving_thread, READY);
        }
        /*if thread was null, nothing is done and loop is exited*/
    }
    while (arriving_thread != NULL);

    return 1;
}

/*this function moves threads from the job queue to the ready queue
  as their arrival time comes*/
int CPU_addArrivingThreadsToReadyQueue(CPUSim * cpu)
{
    Thread * arriving_thread = NULL;

    if (!cpu)
    {
        return -1;
    }

    do
    {
        /*try to remove threads from job_queue which have an arrival time
        equal to the current CPU clock time*/
        arriving_thread = Q_removeThreadAtTime(cpu -> job_queue, cpu -> clock);

        /*if thread was not null, decrement size of job queue*/
        if (arriving_thread != NULL)
        {
            cpu -> job_queue -> size--;

            /*if verbose print as so*/
            if (cpu -> verbose == SET)
            {
                printf("At Time %d: Thread %d of Process %d moves from NEW to READY\n", cpu -> clock, arriving_thread -> thread_number, arriving_thread -> process_number);
            }

            /*add arriving thread to ready queue*/
            CPU_addThread(cpu, arriving_thread, READY);
            /*increase size of ready_queue*/
            cpu -> ready_queue -> size++;
        }
        else
        {
            return 1;
        }
    }
    while (arriving_thread != NULL);

    return 1;
}

/*dispatching function for adding to various queues mananged by CPUSim*/
int CPU_addThread(CPUSim * cpu, Thread * thread, Destination dest)
{
    if(!cpu || !thread)
    {
        return -1;
    }

    /*mode enum signifies which queue to add to*/
    if (dest == READY)
    {
        Q_addThread(cpu -> ready_queue, thread);
    }
    else if (dest == IO)
    {
        Q_addThread(cpu -> io_queue, thread);
    }
    else if (dest == JOB)
    {
        Q_addThread(cpu -> job_queue, thread);
    }
    else
    {
        return -1;
    }

    return 1;

}

/*for telling the CPU when it can cease execution, returns 1 until all threads exit*/
int CPU_canContinue(CPUSim * cpu, Queue * exit_queue)
{
    if (cpu -> num_of_threads != exit_queue -> size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*used to execute a thread, called whenever CPU mode is executing*/
int CPU_executeThreadRR(CPUSim * cpu, Queue * q)
{
    /*EXECUTING can mean either start a new burst or continue on an old one*/
    /*if cpu_is_executing = 0, start executing a new procoess*/
    if (cpu -> cpu_is_executing == 0)
    {
        /*current thread set beforehand*/
        /*set timings sets the length of the CPU and IO bursts to be executed right now*/
        T_setTimings(cpu -> current_thread);
        /*set the CPU wait to the length of the time quantum*/
        cpu -> wait = cpu -> time_quantum;

        /*if this is the first burst in the thread, we set the start time of the thread*/
        if (cpu -> current_thread -> start_time == -1)
        {
            cpu -> current_thread -> start_time = cpu -> clock; /*setting start time to current time*/
        }

        /*if we are not on last burst pair, add the IO time to the threads total*/
        if (cpu -> current_thread -> io_time_remaining >= 0)
        {
            /*add the current IO burst to the total IO done by the thread so far*/
            cpu -> current_thread -> io_thread_total += cpu -> current_thread -> io_time_remaining;
        }

        /*verbose print*/
        if (cpu -> verbose == SET)
        {
             printf("At Time %d: Thread %d of Process %d moves from READY to RUNNING\n", cpu -> clock, cpu -> current_thread -> thread_number, cpu -> current_thread -> process_number);
        }

        /*the cpu is now executing a burst so we chaning the cpu_is_executing to reflect that*/
        cpu -> cpu_is_executing = 1;
    }
    /*if the CPU is in the middle of a burst*/
    else if (cpu -> cpu_is_executing == 1)
    {
        /*decrement the wait (every tick passes is one closer to being done the burst)*/
        cpu -> wait--;
        /*decrement cpu_time as well, because end of time slice, or end of burst means switch*/
        cpu -> current_thread -> cpu_time--;
        /*increase the total amount of cpu execution time*/
        cpu -> total_cpu_execution_time++;
        /*increse the total cpu time of the thread*/
        cpu -> current_thread -> cpu_thread_total++;

        /*if wait (time slice) is done or burst is done,
          we can move this thread out and start on a new one*/
        if (cpu -> wait == 1 || cpu -> current_thread -> cpu_time == 1)
        {
            /*if the io time of the burst is -1, we know that was the last CPU burst
              so we move the current_thread to EXIT*/
            if (cpu -> current_thread -> io_time_remaining == -1)
            {
                /*set exit time of the thread*/
                cpu -> current_thread -> exit_time = cpu -> clock;

                /*add to the queue that holds all exited threads (passed to this function)*/
                Q_addThread(q, cpu -> current_thread);

                /*verbose print*/
                if (cpu -> verbose == SET)
                {
                    printf("At Time %d: Thread %d of Process %d moves from RUNNING to EXIT\n", cpu -> clock, cpu -> current_thread -> thread_number, cpu -> current_thread -> process_number);
                }
            }
            /*add the rest of the burst back to the execution stack*/
            else if (cpu -> wait == 1 && cpu -> current_thread -> cpu_time != 1)
            {
                T_addBurst(cpu -> current_thread, cpu -> current_thread -> cpu_time, cpu -> current_thread -> io_time_remaining);
                /*if not exiting, move the thread to the IO queue so it can do its IO time*/
                if (cpu -> verbose == SET)
                {
                    printf("At Time %d: Thread %d of Process %d moves from RUNNING to READY\n", cpu -> clock, cpu -> current_thread -> thread_number, cpu -> current_thread -> process_number);
                }

                /*add thread to io_queue*/
                CPU_addThread(cpu, cpu -> current_thread, READY);
            }
            else
            {
                /*if not exiting, move the thread to the IO queue so it can do its IO time*/
                if (cpu -> verbose == SET)
                {
                    printf("At Time %d: Thread %d of Process %d moves from RUNNING to BLOCKED\n", cpu -> clock, cpu -> current_thread -> thread_number, cpu -> current_thread -> process_number);
                }


                /*add thread to io_queue*/
                CPU_addThread(cpu, cpu -> current_thread, IO);
            }

            /*after moving the current thread out of cpu, we need a new one so we set the
              cpu mode back to dispatching to get a new one*/
            CPU_setMode(cpu, DISPATCHING);
            /*cpu is no longer executing*/
            cpu -> cpu_is_executing = 0;
        }
    }
    return 1;
}


/*used to execute a thread, called whenever CPU mode is executing*/
int CPU_executeThreadFCFS(CPUSim * cpu, Queue * q)
{
    /*EXECUTING can mean either start a new burst or continue on an old one*/
    /*if cpu_is_executing = 0, start executing a new procoess*/
    if (cpu -> cpu_is_executing == 0)
    {
        /*current thread set beforehand*/
        /*set timings sets the length of the CPU and IO bursts to be executed right now*/
        T_setTimings(cpu -> current_thread);
        /*set the CPU wait to the length of the cpu burst*/
        cpu -> wait = cpu -> current_thread -> cpu_time; /*setting cpu to wait for length of cpu burst (ie do not execute any more threads)*/

        /*if this is the first burst in the thread, we set the start time of the thread*/
        if (cpu -> current_thread -> start_time == -1)
        {
            cpu -> current_thread -> start_time = cpu -> clock; /*setting start time to current time*/
        }

        /*if we are not on last burst pair, add the IO time to the threads total*/
        if (cpu -> current_thread -> io_time_remaining >= 0)
        {
            /*add the current IO burst to the total IO done by the thread so far*/
            cpu -> current_thread -> io_thread_total += cpu -> current_thread -> io_time_remaining;
        }

        /*verbose print*/
        if (cpu -> verbose == SET)
        {
             printf("At Time %d: Thread %d of Process %d moves from READY to RUNNING\n", cpu -> clock, cpu -> current_thread -> thread_number, cpu -> current_thread -> process_number);
        }

        /*the cpu is now executing a burst so we chaning the cpu_is_executing to reflect that*/
        cpu -> cpu_is_executing = 1;
    }
    /*if the CPU is in the middle of a burst*/
    else if (cpu -> cpu_is_executing == 1)
    {
        /*decrement the wait (every tick passes is one closer to being done the burst)*/
        cpu -> wait--;
        /*increase the total amount of cpu execution time*/
        cpu -> total_cpu_execution_time++;
        /*increse the total cpu time of the thread*/
        cpu -> current_thread -> cpu_thread_total++;

        /*if wait is done, eg we can move this thread out and start on a new one*/
        if (cpu -> wait == 1)
        {
            /*if the io time of the burst is -1, we know that was the last CPU burst
              so we move the current_thread to EXIT*/
            if (cpu -> current_thread -> io_time_remaining == -1)
            {
                /*set exit time of the thread*/
                cpu -> current_thread -> exit_time = cpu -> clock;

                /*add to the queue that holds all exited threads (passed to this function)*/
                Q_addThread(q, cpu -> current_thread);

                /*verbose print*/
                if (cpu -> verbose == SET)
                {
                    printf("At Time %d: Thread %d of Process %d moves from RUNNING to EXIT\n", cpu -> clock, cpu -> current_thread -> thread_number, cpu -> current_thread -> process_number);
                }
            }
            else
            {
                /*if not exiting, move the thread to the IO queue so it can do its IO time*/
                if (cpu -> verbose == SET)
                {
                    printf("At Time %d: Thread %d of Process %d moves from RUNNING to BLOCKED\n", cpu -> clock, cpu -> current_thread -> thread_number, cpu -> current_thread -> process_number);
                }

                /*add thread to io_queue*/
                CPU_addThread(cpu, cpu -> current_thread, IO);
            }

            /*after moving the current thread out of cpu, we need a new one so we set the
              cpu mode back to dispatching to get a new one*/
            CPU_setMode(cpu, DISPATCHING);
            /*cpu is no longer executing*/
            cpu -> cpu_is_executing = 0;
        }
    }
    return 1;
}

/*returns the number of processes managed by the CPUSim */
int CPU_getNumberOfProcesses(CPUSim * cpu)
{
    if (!cpu)
    {
        return -1;
    }

    return cpu -> num_of_processes;
}

/*returns the number of threads managed by the CPUSim*/
int CPU_getNumberOfThreads(CPUSim * cpu)
{
    if (!cpu)
    {
        return -1;
    }

    return cpu -> num_of_threads;
}

/*for use in dispatching mode, gets the next thread from the ready queue
  for the cpu to execute*/
int CPU_getNextThread(CPUSim * cpu)
{
    Thread * next_thread = NULL;

    if (!cpu)
    {
        return -1;
    }

    /*grab thread from ready queue*/
    next_thread = Q_removeThread(cpu -> ready_queue);

    if(next_thread != NULL)
    {
        /*sets the current thread of the cpu to the thread pulled from the ready queue
          this thread will be used once the cpu goes into EXECUTING mode*/
        cpu -> current_thread = next_thread;

        /*if the previous thread was from the same process, we do a thread switch*/
        if (cpu -> prev_process == cpu -> current_thread -> process_number)
        {
            /*cpu goes into thread switch mode*/
            CPU_setMode(cpu, TSWITCH);
        }
        else /*if not from the same process, we do a process switch*/
        {
            /*set the new previous process*/
            cpu -> prev_process = cpu -> current_thread -> process_number;
            /*change cpu to process switch mode*/
            CPU_setMode(cpu, PSWITCH);
        }
    }

    return 1;
}

/*advances the master clock and decrements all remaining IO times at the same time*/
void CPU_advanceClock(CPUSim * cpu)
{
    cpu -> clock++;

    Q_decrementAllIO(cpu -> io_queue);
}

/*used to dispatch to the statistics module for calculations*/
void CPU_calculateStatistics(CPUSim * cpu, struct Queue * q)
{
    if (!cpu || !q)
    {
        return;
    }

    cpu -> clock--; /*one extra clock tick upon exit, so removing it here*/

    /*if detailed is set, get detailed statistics*/
    if (cpu -> detailed == SET)
    {
        stats_detailed(cpu, q);
    }
    else
    {
        stats_default(cpu, q);
    }

}

/*used to check when to exit from a TSWITCH or PSWITCH*/
void CPU_checkStatus(CPUSim * cpu)
{
    /*decrement the wait*/
    cpu -> wait--;

    /*if wait is over...*/
    if (cpu -> wait == 0)
    {
        /*move cpu into executing mode*/
        CPU_setMode(cpu, EXECUTING);
        cpu -> wait = 0;
    }
}

/*dispatcher for different execution algorithms based off scheduling descisions*/
void CPU_executeThread(CPUSim * cpu, Queue * q)
{
    if (cpu -> round_robin = SET)
    {
        CPU_executeThreadRR(cpu, q);
    }
    else
    {
        CPU_executeThreadFCFS(cpu,q);
    }
}

/*sets the mode of the CPU*/
void CPU_setMode(CPUSim * cpu, Mode mode)
{
    if (mode == TSWITCH)
    {
        /*if we are switching into threadswitch mode, the cpu wait is set to
          the length of thread switch parsed from file*/
        cpu -> wait = cpu -> thread_switch;
    }
    else if (mode == PSWITCH)
    {
        /*same for process switch*/
        cpu -> wait = cpu -> process_switch;
    }

    /*set the mode now*/
    cpu -> mode = mode;
}








