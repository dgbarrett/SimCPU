#include "Thread.h"

/*the thread type is the unit that is passed around between
  various queues in the CPUSim (job, io, ready)*/

/*creates and initializes a new Thread*/
Thread * T_create(int process_number, int thread_number, int arrival_time, int cpu_bursts)
{
    Thread * new_thread = malloc(sizeof(Thread));

    if (!new_thread)
    {
        MEMORY_ALLOCATION_ERROR("Thread *", "T_create", "Thread.c");
        return NULL;
    }

    /*set all to their default/assigned values*/
    new_thread -> process_number = process_number;
    new_thread -> thread_number = thread_number;

    new_thread -> cpu_time = 0;
    new_thread -> cpu_thread_total = 0;
    new_thread -> io_thread_total = 0;
    new_thread -> io_time_remaining = 0;

    new_thread -> arrival_time = arrival_time;
    new_thread -> start_time = -1;
    new_thread -> exit_time = DEFAULT_EXIT_VALUE;

    new_thread -> bursts = cpu_bursts;
    /*create a new execution stack when creating new thread*/
    new_thread -> burst_queue = BQ_create();
    new_thread -> next = NULL;

    if (new_thread -> burst_queue == NULL)
    {
        return NULL;
    }

    return new_thread;
}

/*add a new burst to the execution stack of a given thread*/
int T_addBurst(Thread * thread, int cpu_time, int io_time)
{
    if(!thread)
    {
        return -1;
    }

    /*use lower-level function to execute*/
    if(BQ_enqueue(thread -> burst_queue, cpu_time, io_time))
    {
        return 1;
    }

    return -1;
}

/*gets the number of cpu-io burst pairs in the execution stack*/
int T_getNumberOfBursts(Thread * thread)
{
    if (!thread)
    {
        return -1;
    }

    return thread -> bursts;
}

/*pops a burst pair off execution stack and sets the cpu_time
 and io_time_remaining variables in the thread for use in that
 execution sequeuence*/
int T_setTimings(Thread * thread)
{
    if(!thread)
    {
        return -1;
    }

    /*pop burst off execution stack*/
    BurstNode * bn = BQ_dequeue(thread -> burst_queue);

    /*setting thread variables based on popped burst*/
    thread -> cpu_time = bn -> cpu_time;
    thread -> io_time_remaining = bn -> io_time;

    return 1;
}

/*print the contents of a thread*/
void T_print(Thread * thread)
{
    if (!thread)
    {
        return;
    }

    printf("IO Time remaining: %d\n", thread -> io_time_remaining);
    printf("Process Number: %d\n", thread -> process_number);
    printf("Thread Number: %d\n", thread -> thread_number);
    printf("Arrival Time: %d\n", thread -> arrival_time);
    printf("Start Time: %d\n", thread -> start_time);
    printf("Exit Time: %d\n", thread -> exit_time);
    printf("Total CPU time: %d\n", thread -> cpu_thread_total);
    printf("Total IO time: %d\n", thread -> io_thread_total);
    printf("CPU Bursts: %d\n", thread -> bursts);
    BQ_print(thread -> burst_queue);
    printf("\n");
}

/*destroy and deallocate a thread*/
void T_destroy(Thread * thread)
{
    if (!thread)
    {
        return;
    }

    /*only burst queue is dynamically allocated in thread*/
    BQ_destroy(thread -> burst_queue);

    free(thread);
}

/*int main()
{
    Thread * t = T_create(1, 2, 230, 5);
    T_addBurst(t, 1,2);
    T_addBurst(t, 3,4);
    T_setTimings(t);
    T_setTimings(t);
    T_print(t);
    return 0;
}*/
