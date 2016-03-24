#include "stats.h"

/*prints the final stats of the CPUSim in defualt mode*/
void stats_default(CPUSim * cpu, Queue * q)
{
    if(!cpu || !q)
    {
        return;
    }

    float cpu_util = 0;
    int total_time = cpu -> clock;

    /*calculate cpu utilization*/
    cpu_util = ((float)cpu -> total_cpu_execution_time / cpu -> clock)*100;

    /*pass to print function*/
    printDefaultStats(cpu, q, cpu_util, total_time);
}

/*prints the final stats of the CPUSim in detailed mode, threads presented in exit order*/
void stats_detailed(CPUSim * cpu, Queue * q)
{
    /*default stats are part of the detailed stats*/
    stats_default(cpu, q);

    /*temp thread points to head of the exit_queue which contains all exited threads*/
    Thread * temp = q -> head;

    while (temp != NULL)
    {
        /*print detailed info*/
        printf("\n");
        printf("Thread %d of Process %d:\n\n", temp -> thread_number, temp -> process_number);
        printf("arrival time: %d\n", temp -> arrival_time);
        printf("service time: %d\n", temp -> cpu_thread_total);
        printf("I/O time: %d\n", temp -> io_thread_total);
        printf("turnaround time: %d\n", temp -> exit_time - temp -> arrival_time);
        printf("exit time: %d\n", temp -> exit_time);
        printf("\n");

        /*move to next item in exit queue*/
        temp = temp -> next;
    }
}

/*printing function*/
void printDefaultStats(CPUSim * cpu, Queue * q, float cpu_util, int time)
{
    /*chooses which mode to print out based on what mode the cpu executed in*/
    if (cpu -> time_quantum != -1)
    {
        printf("\nRound Robin (with time quantum = %d): \n\n", cpu -> time_quantum);
    }
    else
    {
        printf("\nFCFS:\n\n");
    }

    /*printing default stats*/
    printf("Total Time required is %d time units\n", time);
    printf("Average Turnaround Time is %.1f time units\n", turnaroundTime(cpu, q));
    printf("CPU Utilization is %.0f percent\n\n", cpu_util);
}

float turnaroundTime(CPUSim * cpu, Queue * q)
{
    int loop_var = 0;
    float arr_temp = 0;
    float exit_temp = 0;
    float turnaround = 0;
    Thread * temp = NULL;

    for (loop_var = 1; loop_var <= cpu -> num_of_processes; loop_var++)
    {
        temp = q -> head;
        arr_temp = exit_temp = 0;

        while (temp != NULL)
        {
            if(temp -> process_number == loop_var)
            {
                arr_temp = temp -> arrival_time;
                exit_temp = temp -> exit_time;
            }

            temp = temp -> next;
        }

        turnaround += (exit_temp - arr_temp);
    }

    return turnaround / (cpu -> num_of_processes);
}

