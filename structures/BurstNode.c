#include "BurstNode.h"
/*BurstNode type is the node type for the execution stack/list/queue/thing*/

/*allocate space for and initialize a BurstNode*/
BurstNode * BN_create(int cpu_time, int io_time)
{
    BurstNode * new_bn = malloc(sizeof(BurstNode));

    if (!new_bn)
    {
        return NULL;
    }

    new_bn -> cpu_time = cpu_time;
    new_bn -> io_time = io_time;
    /*default next points to NULL*/
    new_bn -> next = NULL;

    return new_bn;
}

/*returns the cpu time stored within a burst node*/
int BN_getCPUTime(BurstNode * node)
{
    if (!node)
    {
        return -1;
    }

    return node -> cpu_time;
}

/*returns the io time stored within a burst node*/
int BN_getIOTime(BurstNode * node)
{
    if (!node)
    {
        return -1;
    }

    return node -> io_time;
}

/*prints the values contained in a burst node*/
void BN_print(BurstNode * node)
{
    if(!node)
    {
        return;
    }

    printf("\tcpu: %d \t io: %d \n", BN_getCPUTime(node), BN_getIOTime(node));
}

/*destroys and deallocates a burst node*/
void BN_destroy(BurstNode * node)
{
    if (!node)
    {
        return;
    }

    free(node);
    node = NULL;
}


