#ifndef __BURSTNODE_H__
#define __BURSTNODE_H__

#include <stdio.h>
#include <stdlib.h>
#include "../error/errors.h"

/*the BurstNode type holds a pair of bursts (1 io, and 1 cpu) on the execution stack (BurstQueue)*/

typedef struct BurstNode {
    int cpu_time;               /*holds the length of a cpu burst on the execution stack*/
    int io_time;                /*holds length of an io burst on the execution stack*/
    struct BurstNode * next;    /*points to next burst pair in the execution stack*/
} BurstNode;

BurstNode *     BN_create           (int cpu_time, int io_time);
int             BN_getCPUTime       (BurstNode * node);
int             BN_getIOTime        (BurstNode * node);
void            BN_destroy          (BurstNode * node);
void            BN_print            (BurstNode * node);

#endif /* __BURSTNODE_H__ */
