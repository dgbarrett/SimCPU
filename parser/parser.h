#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../structures/CPUSim.h"
#include "../structures/Thread.h"
#include "../structures/Queue.h"
#include "../structures/BurstQueue.h"
#include "../structures/BurstNode.h"
#include "../error/errors.h"

int         parseCPUInfo                (CPUSim * cpu);
int         parseBursts                 (Thread * thread);
int         parseThreads                (CPUSim * cpu, int process_num);
int         parseThread                 (CPUSim * cpu, int process_num);
int         parseProcesses              (CPUSim * cpu);
int         initializeJobQueue          (CPUSim * cpu);
void        processCommandLineArgs      (CPUSim * cpu, char ** argv, int argc);


#endif /* __PARSER_H__ */
