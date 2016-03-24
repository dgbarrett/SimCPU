#include "parser.h"

/*this function reads all threads from the file and adds them to the job queue,
 the job queue feeds the ready queue new threads*/
int initializeJobQueue(CPUSim * cpu)
{
    if (!cpu)
    {
        return -1;
    }

    parseCPUInfo(cpu); /*parses the first line of the file bc it does not show up in the file pattern again*/

    parseProcesses(cpu); /*parse all processes in the file, based off of info from parseCPUInfo*/

    /*after all processes are parsed, set the number of threads the cpu has, to the size
    of the job queue*/
    cpu -> num_of_threads = cpu -> job_queue -> size;

    return 1;
}

/*responsible for parsing all processes and their threads in file*/
int parseProcesses(CPUSim * cpu)
{
    int process_num = 0;
    int loop_var = 0;

    /*for the number of processes in the file...*/
    for (loop_var = 0; loop_var < cpu -> num_of_processes; loop_var++)
    {
        /*scan in process number, and num of threads in said process*/
        scanf("%d %d", &process_num, &(cpu -> num_of_threads));
        /*parse threads based off number of threads in process*/
        parseThreads(cpu, process_num);
    }

    return 1;
}

/*responsible for parsing all threads in a given process*/
int parseThreads(CPUSim * cpu, int process_num)
{
    int loop_var = 0;

    /*for all threads in a process...*/
    for(loop_var = 0; loop_var < CPU_getNumberOfThreads(cpu); loop_var++)
    {
        /*parse a thread*/
        parseThread(cpu, process_num);
    }

    return 1;
}

/*parses one thread from file*/
int parseThread(CPUSim * cpu, int process_num)
{
    int thread_number = 0;
    int arrival_time = -1;
    int num_of_bursts = 0;
    Thread * new_thread = NULL;

    /*scan in thread num wrt to process, its arrival time, and number of cpu burts*/
    scanf("%d %d %d", &thread_number, &arrival_time, &num_of_bursts);

    /*create a new thread object with parsed info*/
    new_thread = T_create(process_num, thread_number, arrival_time, num_of_bursts);

    /*parse the execution stack of the thread based on 'num_of_bursts'*/
    parseBursts(new_thread);

    /*add the thread to the job queue inside our cpu sim*/
    CPU_addThread(cpu, new_thread, JOB);

    return 1;
}

/*parses the execution stack of one thread*/
int parseBursts(Thread * thread)
{
    int loop_var = 0;
    int burst_num = 0;
    int cpu_time = 0;
    int io_time = 0;

    if (!thread)
    {
        return -1;
    }

    /*for x-1 number of bursts in the stack...*/
    for(loop_var = 0; loop_var < (T_getNumberOfBursts(thread) - 1); loop_var++)
    {
        /*scan in info*/
        scanf("%d %d %d", &burst_num, &cpu_time, &io_time);
        /*add info to execution stack object (BurstQueue) inside the already created thread*/
        T_addBurst(thread, cpu_time, io_time);
    }

    /*scan in the last burst seperatley, because we expect the last burst to have no io*/
    scanf("%d %d", &burst_num, &cpu_time);
    /*set last io burst to -1 for signal use later*/
    T_addBurst(thread, cpu_time, -1);

    return 1;
}

/*parses the first line of the file*/
int parseCPUInfo(CPUSim * cpu)
{
    if (!cpu)
    {
        return -1;
    }

    scanf("%d %d %d", &(cpu -> num_of_processes), &(cpu -> thread_switch), &(cpu -> process_switch));

    return 1;
}

/*responsible for setting flags inside CPUSim object to set output style, scheduling etc...*/
void processCommandLineArgs(CPUSim * cpu, char ** argv, int argc)
{
    int loop_var = 0;

    /*make sure there are not too many arguements on the cmd line, exit if there are too many*/
    if (argc > 7)
    {
        printf("Invalid command line parameters. Exiting.\n");
        exit(0);
    }

    /*for all args in argv...*/
    for (loop_var = 0; loop_var < argc ; loop_var++)
    {
        /*compare arg with flag*/
        if (strcmp(argv[loop_var], "-d") == 0)
        {
            /*if flag recognized, set corresponding flag in CPUSim object*/
            cpu -> detailed = SET;
            break;
        }
    }

    for (loop_var = 0; loop_var < argc ; loop_var++)
    {
        if (strcmp(argv[loop_var], "-v") == 0)
        {
            cpu -> verbose = SET;
            break;
        }
    }

    for (loop_var = 0; loop_var < argc ; loop_var++)
    {
        if (strcmp(argv[loop_var], "-r") == 0)
        {
            cpu -> round_robin = SET;
            break;
        }
    }

    /*for all args in argv...*/
    for (loop_var = 0; loop_var < argc ; loop_var++)
    {
        /*check the first letter to see if it is a digit*/
        if (isdigit(argv[loop_var][0]))
        {
            /*if it is, turn that args into an integer, and set time quantum in CPUSim*/
            int num = atoi(argv[loop_var]);
            cpu -> time_quantum = num;
            break;
        }
    }
}
/*int main ()
{
    CPUSim * cpu = CPU_create(SET,SET,SET,100);
    initializeJobQueue(cpu);

    printf("threads: %d\n", cpu -> num_of_threads);



    return 0;
}*/




/*int main ()
{
    CPUSim * cpu = CPU_create(SET,SET,SET,100);
    Thread *t = NULL;

    initializeJobQueue(cpu);


    return 0;
}*/
