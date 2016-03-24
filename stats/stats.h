#ifndef __STATS_H__
#define __STATS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "../parser/parser.h"
#include "../structures/CPUSim.h"
#include "../structures/Thread.h"
#include "../structures/Queue.h"
#include "../structures/BurstQueue.h"
#include "../structures/BurstNode.h"
#include "../error/errors.h"

float       turnaroundTime      (CPUSim * cpu, Queue * q);
void        stats_default       (CPUSim * cpu, Queue * q);
void        stats_detailed      (CPUSim * cpu, Queue * q);
void        printDefaultStats   (CPUSim * cpu, Queue * q, float cpu_util, int time);

#endif /* __STATS_H__*/
