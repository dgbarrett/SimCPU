#include "simcpu.h"

int main (int argc, char ** argv)
{
    CPUSim * cpu = CPU_create();
    Queue * exit_queue = Q_create();

    processCommandLineArgs(cpu, argv, argc); /*sets flags and/or time quantum*/

    initializeJobQueue(cpu);

    while(CPU_canContinue(cpu, exit_queue)) /*if there are still threads to be worked on continue*/
    {
        switch (cpu -> mode)
        {
            case NEWCPU: /*CPU in NEWCPU upon initialization*/
                CPU_setMode(cpu, DISPATCHING);
                break;
            case DISPATCHING:
                /*if cpu dispatching, get the next thread to execute,
                  this function auto switches to either PSWITCH or
                  TSWITCH cpu mode based on the circumstanses*/
                CPU_getNextThread(cpu);
                break;
            case EXECUTING:
                /*executes a burst or loads in a new one if there is not one executing*/
                /*function auto switches to dispatching once a thread is done its burst*/
                CPU_executeThread(cpu, exit_queue);
                break;
            case PSWITCH:
            case TSWITCH:
                /*check to see when we can exit the context switch and begin executing*/
                CPU_checkStatus(cpu);
                break;
            default:
                printf("Fatal Error. Exiting\n");
                exit(0);
        }

        /*move any arriving threads into ready queue*/
        CPU_addArrivingThreadsToReadyQueue(cpu);
        /*move any finished IO threads to ready queue*/
        CPU_addFinishedIOThreadsToReadyQueue(cpu);

        /*clock tick*/
        CPU_advanceClock(cpu);
    }

    /*once all threads exit we calculate and display stats*/
    CPU_calculateStatistics(cpu, exit_queue);

    return 0;
}
