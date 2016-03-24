#include "BurstQueue.h"

/*the BurstQueue type is effectivley the execution stack of a thread, I called it
a queue because I am loading it in a FIFO style*/

/*creates and initializes a new BurstQueue*/
BurstQueue * BQ_create()
{
    BurstQueue * new_bq = malloc(sizeof(BurstQueue));

    if (!new_bq)
    {
        MEMORY_ALLOCATION_ERROR("BurstQueue *", "BQ_create", "BurstQueue.c");
        return NULL;
    }

    /*head and tail both null upon allocation*/
    new_bq -> head = new_bq -> tail = NULL;

    return new_bq;
}

/*add a BurstNode to the BurstQueue by passing two int values (BurstNode created internally*/
int BQ_enqueue(BurstQueue * bq, int cpu_time, int io_time)
{
    if(!bq)
    {
        return -1;
    }

    /*if head of queue is null*/
    if (!BQ_getHead(bq))
    {
        /*set the head*/
        BQ_setHead(bq, cpu_time, io_time);
        return 1;
    }
    else
    {
        /*otherwise add at the end of the queue*/
        BQ_setTail(bq, cpu_time, io_time);
        return 1;
    }
}

/*remove the head from a BurstQueue*/
BurstNode * BQ_dequeue(BurstQueue * bq)
{
    BurstNode * temp = NULL;

    if(!bq)
    {
        return NULL;
    }

    /*we will return temp*/
    temp = bq -> head;

    if (bq -> head -> next != NULL)
    {
        /*move the bq pointer to head onto the one after the current head*/
        bq -> head = bq -> head -> next;
    }

    /*return the old head*/
    return temp;
}

/*for internal use in setting the head of a BurstQueue*/
int BQ_setHead(BurstQueue * bq, int cpu_time, int io_time)
{
    /*create new burst node out of passed ints*/
    BurstNode * new_bn = BN_create(cpu_time, io_time);

    if(!new_bn)
    {
        return -1;
    }

    /*because queue was empty, new_bn is now head and tail of the list*/
    bq -> head = bq -> tail = new_bn;

    return 1;
}

/*for internal use in setting the tail of a BurstQueue*/
int BQ_setTail(BurstQueue * bq, int cpu_time, int io_time)
{
    /*create a new BurstNode out of passed ints*/
    BurstNode * new_bn = BN_create(cpu_time, io_time);

    if(!new_bn)
    {
        return -1;
    }

    /*new node is equal to the node after the current tail*/
    bq -> tail -> next = new_bn;
    /*tail now points to the node after the old tail*/
    bq -> tail = bq  -> tail -> next;

    return 1;
}

/*returns the head of a BurstQueue*/
BurstNode * BQ_getHead(BurstQueue * bq)
{
    if(!bq)
    {
        return NULL;
    }

    return bq -> head;
}

/*prints the contents of all nodes in a BurstQueue*/
void BQ_print(BurstQueue * bq)
{
    BurstNode * temp = BQ_getHead(bq);

    while(temp)
    {
        BN_print(temp);
        temp = temp -> next;
    }
}

/*deallocates and destroys a BurstQueue*/
void BQ_destroy(BurstQueue * bq)
{
    BurstNode * temp = BQ_getHead(bq);

    if (!bq)
    {
        return;
    }

    while (!temp)
    {
        /*move head down one*/
        bq -> head = bq -> head -> next;
        free(temp);
        /*temp equal to the new head*/
        temp = bq -> head;
    }

    free(bq);
}

/*int main()
{
    BurstQueue * q = BQ_create(10);
    BurstNode * n = NULL;
    BQ_enqueue(q, 1 ,2);
    BQ_enqueue(q, 12 ,2);
    BQ_enqueue(q, 3 ,2);
    BQ_enqueue(q, 4 ,2);

    n = BQ_dequeue(q);
    n = BQ_dequeue(q);

    printf("%d\n", n->cpu_time);
    BQ_print(q);
    BQ_destroy(q);
    return 0;
}*/
