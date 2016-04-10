SimCPU
=======

SimCPU is a CPU Scheduling Simulation that completes the "execution" of a group of multi-threaded processes.  Each process consists of 1-50 threads, and each thread has its own CPU and IO time requirements.  SimCPU uses thread-level scheduling to schedule and "execute" all processes as efficiently as possible.



Building
---------

- cd to the 'main' directory within the project folder  
- once in main type 'make simcpu'
- ignore any warnings (TODO)



Usage
------

- invoke simcpu by "simcpu [-d] [-v] [-r quantum] < input_file"
	- "-d" outputs detailed statistics about the CPU's exectution including average turaround time for each process, CPU utilization, and various information about each thread.

	- "-v" enables verbose mode. In this mode, the result of every scheduling descion is printed to STDOUT.
		
	- "-r" enables round robin CPU scheduling with the given time quantum (an integer).  Default scheduling mode is "First Come First Server" (FCFS).



Input Format
------------
An input file has the format:

*******************************************************************************

num_of_processes thread_switch_overhead_time process_switch_overhead_time

process_number num_of_threads

thread_number arrival_time num_CPU_bursts
1 cpu_time io_time
2 cpu_time io_time # these are CPU bursts

...

*******************************************************************************

see /test for examples.



Assumptions
-----------

The input redirection indicator (<) and input file are always the last command lines argument(s)

There are spaces surrounding the redirection indicator (eg. "cpusim -d -v < file.txt", rather than "cpusim -d -v < file.txt")

There are no comments in the input files

The time quantum value is seperated from the -r flag by a space (eg "cpusim -r 100 < file.txt")

All command line arguments are seperated by spaces




