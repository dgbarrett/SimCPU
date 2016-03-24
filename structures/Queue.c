#include "Queue.h"

/*Queues are used to control the flows of threads through the simulation*/

/*allocates and initializes a new Queue*/
Queue * Q_create()
{
    Queue * new_q = malloc(sizeof(Queue));

    if (!new_q)
    {
        MEMORY_ALLOCATION_ERROR("Queue *", "Q_create", "Queue.c");
        return NULL;
    }

    /*start with an empty queue*/
    new_q -> head = new_q -> tail = NULL;
    new_q -> size = 0;

    return new_q;
}

/*add a thread to the queue*/
int Q_addThread(Queue * q, Thread * thread)
{
    if(!thread || !q)
    {
        return -1;
    }

    /*increase the size of the queue when adding a new non-null thread*/
    q -> size++;

    /*if head is null*/
    if (!Q_getHead(q))
    {
        Q_setHead(q, thread);
        return 1;
    }
    else
    {
        Q_setTail(q, thread);
        return 1;
    }
}

/*removes the thread at the front of the queue*/
Thread * Q_removeThread(Queue * q)
{
    Thread * temp = NULL;

    if (!q || !q -> head)
    {
        return NULL;
    }

    temp = q -> head;
    q -> head = q -> head -> next;
    temp -> next = NULL;

    return temp;
}

/*removes the first thread in the queue with arrival_time = incoming 'time' variable
  this is used to move threads from the job queue to the ready queue as they arrive*/
Thread * Q_removeThreadAtTime(Queue * q, int time)
{
    Thread * temp;
    Thread * prev;

    if (q == NULL || q->head == NULL)
    {
        return NULL;
    }

    /*start with the head of the queue*/
    temp = q -> head;

    while (temp != NULL)
    {
        /*if times match...*/
        if (temp -> arrival_time == time)
        {
            /*if temp is the head of the list...*/
            if(temp == q -> head)
            {
                /*incrememnt head*/
                q -> head = q -> head -> next;
                temp -> next = NULL;
                return temp;
            }
            else
            {
                /*previous node points to the next node of the one we are removing*/
                prev -> next = temp -> next;
                temp -> next = NULL;
                return temp;
            }
        }

        /*increment prev and temp every loop iteration*/
        prev = temp;
        temp = temp -> next;

    }

    return NULL;
}

/*removes threads from the IO queue when they are done their IO time*/
Thread * Q_removeIOThreadAtTime(Queue *q, int io_time_finished)
{
    Thread * temp;
    Thread * prev;

    if (!q)
    {
        return NULL;
    }

    temp = q -> head;

    /*same procedure as above*/
    while (temp != NULL)
    {
        if (temp -> io_time_remaining == io_time_finished)
        {
            if(temp == q -> head)
            {
                q -> head = q -> head -> next;
                 temp -> next = NULL;
                return temp;
            }
            else
            {
                prev -> next = temp -> next;
                 temp -> next = NULL;
                return temp;
            }
        }

        prev = temp;
        temp = temp -> next;
    }

    return NULL;

}

/*gets the head of a Queue*/
Thread * Q_getHead(Queue * q)
{
    if (!q)
    {
        return NULL;
    }

    return q -> head;
}

/*decrements the remaining io time of every item in IO queue*/
void Q_decrementAllIO(Queue * q)
{
    Thread * temp = q -> head;

    while (temp != NULL)
    {
        temp -> io_time_remaining--;
        temp = temp -> next;
    }
}

/*for setting the head of a Queue*/
void Q_setHead(Queue *q, Thread * thread)
{
    q -> head = q -> tail = thread;
}

/*for setting the tail of a Queue*/
void Q_setTail(Queue * q, Thread * thread)
{
    q -> tail -> next = thread;
    q -> tail = q  -> tail -> next;
    q -> tail -> next = NULL;
}

/*prints the contenets of all threads in a Queue*/
void Q_print(Queue * q)
{
    Thread * temp = Q_getHead(q);

    while(temp)
    {
        T_print(temp);
        temp = temp -> next;
    }
}

/*destroys and deallocates a Queue*/
void Q_destroy(Queue * q)
{
    Thread * temp = Q_getHead(q);

    if (!q)
    {
        return;
    }

    while (!temp)
    {
        q -> head = q -> head -> next;
        free(temp);
        temp = q -> head;
    }

    free(q);
}

/*int main()
{
    Queue * q = Q_create();

    Thread * t = T_create(1, 2, 230, 5);
    Thread * t2 = T_create(1,3, 250, 3);
    Thread * t3 = T_create (1,4, 250, 1);
    Thread * t4;

    T_addBurst(t, 1, 2);
    T_addBurst(t, 3, 4);
    T_addBurst(t2, 4, 5);
    T_addBurst(t2, 6, 7);
    T_addBurst(t3, 8,9);

    T_setTimings(t);
    T_setTimings(t2);
    T_setTimings(t3);


    Q_addThread(q, t);
    Q_addThread(q, t2);
    Q_addThread(q, t3);

    Q_decrementAllIO(q);

    Q_print(q);

    return 0;
}*/
