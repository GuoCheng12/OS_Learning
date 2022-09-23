static void *THREAD_Remove (Queue *queue)
{
    Node Head;
    void *ret;
    
    Head=queue->Head;
    if (Head==NULL)                    /* if empty, return NULL    */
    return (void *)Head;
    queue->Head=Head->Next;            /* reposition Head pointer  */
    if (queue->Head == NULL)           /* if queue becomes empty   */
    queue->Tail = NULL;           /* then Head=Tail=NULL      */
    ret = Head->Content;
    free(Head);
    return ret;
}

/* ---------------------------------------------------------------- */
/* THREAD_RemovefromQueue:                                          */
/*   Removes and returns the thread with a specified ID.            */
/*                                                                  */
/* Arguments:                                                       */
/*   void  *ID      -- the ID to be removed                         */
/*   Queue *queue   -- the input queue                              */
/* Return value:                                                    */
/*   mtu_ERROR      -- Failure                                      */
/*   mtu_NORMAL     -- Success                                      */
/* ---------------------------------------------------------------- */

static int THREAD_RemovefromQueue (Queue *queue, void *ID)
{
    Node Temp1, Temp2;
    
    Temp1 = queue->Head;
    Temp2 = queue->Head;
    if (Temp1 == NULL)
    return mtu_ERROR;
    
    if ((((SEM_t)ID)->Magic != mtu_VALID) &&  /* valid ID?         */
        ((((THREAD_t)ID)->Name < 0) ||
         (((THREAD_t)ID)->Name > NextThreadName)))  {
            mtuMTP_errno = mtuMTP_INVALIDID;
            return mtu_ERROR;
        }
    
    if (Temp1->Content == ID) {        /* ID the Head of queue?    */
        if (queue->Tail == Temp1) {   /* the only ID?             */
            free(queue->Head);       /* then remove it           */
            queue->Head=queue->Tail=NULL;
        }
        else {                        /* otherwise, remove head   */
            queue->Head = Temp1->Next;
            free(Temp1);
        }
        return mtu_NORMAL;
    }
    
    while ((Temp2 = Temp1->Next) != NULL) {  /* ID not the head    */
        if (Temp2->Content == ID) {   /* search the queue for ID  */
            if (queue->Tail == Temp2)
            queue->Tail = Temp1;
            Temp1->Next = Temp2->Next;
            free(Temp2);
            return mtu_NORMAL;
        }
        Temp1=Temp2;
    }
    mtuMTP_errno = mtuMTP_NOTINQUEUE;  /* return mtu_ERROR         */
    return mtu_ERROR;
}

/* ---------------------------------------------------------------- */
/* THREAD_Append:                                                   */
/*   Append an element to the Tail of the queue                     */
/*                                                                  */
/* Arguments:                                                       */
/*   Queue* queue   -- the input queue                              */
/*   void   *ID     -- the ID to be added                           */
/* Return value:                                                    */
/*   mtu_ERROR      -- Failure                                      */
/*   mtu_NORMAL     -- Success                                      */
/* ---------------------------------------------------------------- */

static int THREAD_Append (Queue *queue, void *ID)
{
    if ((((SEM_t)ID)->Magic != mtu_VALID) &&  /* a valid ID?       */
        ((((THREAD_t)ID)->Name < 0) ||
         (((THREAD_t)ID)->Name > NextThreadName))) {
            mtuMTP_errno=mtuMTP_INVALIDID;
            return mtu_ERROR;
        }
    if (queue->Tail == NULL) {         /* if the queue is empty    */
        queue->Head = (Node)malloc(sizeof(NODE));
        if (queue->Head == NULL)
        return mtu_ERROR;
        queue->Head->Content = ID;
        queue->Tail=queue->Head;
    }
    else {                             /* otherwise, append ID     */
        queue->Tail->Next=(Node)malloc(sizeof(NODE));
        if (queue->Tail->Next == NULL)
        return mtu_ERROR;
        queue->Tail->Next->Content = ID;
        queue->Tail=queue->Tail->Next;
    }
    queue->Tail->Next = NULL;
    return mtu_NORMAL;
}

/* ---------------------------------------------------------------- */
/* Remove: Same as THREAD_Remove                                    */
/*   Removes and returns the Head of the queue.                     */
/*                                                                  */
/* Argument:                                                        */
/*   Queue *queue -- the input queue                                */
/* Return value:                                                    */
/*   NULL         -- if the input queue is empty                    */
/*   queue's Head -- otherwise                                      */
/* Nov 7th 2005 : Revised by TinTin Yu                              */
/* ---------------------------------------------------------------- */
static void *Remove (Queue *queue)
{
    return THREAD_Remove(queue);
}

/* ---------------------------------------------------------------- */
/* Append                                                           */
/*   Modified from THREAD_Append on December 27, 2003. This enables */
/*   the program to use Append to use queues that do not            */
/*   necessarily use threads.                                       */
/*                                                                  */
/* Arguments:                                                       */
/*   Queue *queue   -- Queue to be appended                         */
/*   void *ID       -- The item appended                            */
/* Return Value:                                                    */
/*   mtu_ERROR      -- Failure                                      */
/*   mtu_NORMAL     -- Success                                      */
/* ---------------------------------------------------------------- */
static int Append(Queue *queue, void *ID) {
    if (queue->Tail == NULL) {          /* if the queue is empty    */
        
        queue->Head = (Node)malloc(sizeof(NODE));
        if (queue->Head == NULL)
        return mtu_ERROR;
        queue->Head->Content = ID;
        queue->Tail=queue->Head;
        
    } else {                            /* otherwise, append ID     */
        queue->Tail->Next=(Node)malloc(sizeof(NODE));
        if (queue->Tail->Next == NULL)
        return mtu_ERROR;
        queue->Tail->Next->Content = ID;
        queue->Tail=queue->Tail->Next;
    }
    queue->Tail->Next = NULL;
    return mtu_NORMAL;
}
