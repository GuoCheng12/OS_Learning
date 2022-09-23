#if !defined(SOLARIS) && !defined(SUN4) && !defined(LINUX) && !defined(WIN32)
#define LINUX
#endif

/* ---------------------------------------------------------------- */
/* THREAD_INIT():                                                   */
/*   Initializes a new thread's environment.                        */
/*                                                                  */
/* Arguments:                                                       */
/*   NewThread      -- the ID (pointer of a TCB structure) of the   */
/*                     new thread                                   */
/*   StackPointer   -- the stack pointer of the new thread          */
/* Method:                                                          */
/*   This function first saves the current environment into         */
/* PreserveThr.  Then, it sets up an environment into the running   */
/* (actually the newly created) thread's environment table.         */
/* This is followed by creating a new environment.  Here is how.    */
/*   (1)  the instruction "ta" is used to force an interrupt so that*/
/*        the current environment is saved to the stack             */
/*   (2)  the instruction "ld" is used to load the new stack pointer*/
/*   (3)  then the new stack pointer is saved.  as a result, the    */
/*        can use the stack pointer saved "there"                   */
/* Finally, the original environment is restored, followed by a call*/
/* to THREAD_WRAP(), which calls the newly created thread that will */
/* use the setup stack pointer.                                     */
/* ---------------------------------------------------------------- */

#if defined(SOLARIS) || defined(SUN4)
void THREAD_INIT (volatile TCB *volatile NewThread, void *StackPointer)
{
    static jmp_buf *PreserveThr;
    
    /* preserve PreserveThr for new Environment */
    PreserveThr = (jmp_buf *) &NewThread->Environment;
    if ((SaveEnvironment (Running->Environment) == 0))
    {
        /* create a new Environment */
        asm volatile ("ta       0x03"); /* flush regs to stack    */
        asm volatile ("ld       %0, %%o0": "=o" (StackPointer));
        /* load new stack pointer   */
        /* StackPointer             */
        asm volatile ("save     %o0,-96, %sp");
        /* move register window and */
        /* save the stack pointer   */
        if (SaveEnvironment (*PreserveThr) == 0)   /* save new env*/
        RestoreEnvironment(Running->Environment);/* load env */
        THREAD_WRAP ();               /* run the new thread       */
    }
}
#endif

#ifdef LINUX
void THREAD_INIT (volatile TCB *volatile NewThread, void *StackPointer)
{
    /* In Linux 1.0 the code maybe like following three lines
     *    NewThread->Environment->__sp = StackPointer;
     *    NewThread->Environment->__bp = StackPointer;
     *    NewThread->Environment->__pc = (void *)THREAD_WRAP;
     * Here is THREAD_INIT for Linux 2.0
     */
    NewThread->Environment[0].__jmpbuf[JB_SP] = (int)StackPointer;
    NewThread->Environment[0].__jmpbuf[JB_BP] = (int)StackPointer;
    NewThread->Environment[0].__jmpbuf[JB_PC] = (int)THREAD_WRAP;
}
#endif

#ifdef WIN32
void THREAD_INIT (volatile TCB *volatile NewThread, void *StackPointer)
{
    static jmp_buf *PreserveThr;
    
    /* preserve PreserveThr for new Environment */
    PreserveThr = (jmp_buf *)&NewThread->Environment;
    if ((SaveEnvironment (Running->Environment) == 0))
    {
        /* change the value of register sp */
        __asm mov    esp, dword ptr [ebp+0Ch];
        if (SaveEnvironment (*PreserveThr) == 0)
        RestoreEnvironment (Running->Environment);
        THREAD_WRAP ();
    }
}
#endif

/* ---------------------------------------------------------------- */
/* THREAD_SYS_INIT:                                                 */
/*      Initializes the coroutine structures                        */
/*                                                                  */
/* Return value:                                                    */
/*      mtu_ERROR       -- Failure                                  */
/*   mtu_NORMAL         -- Success                                  */
/* ---------------------------------------------------------------- */
static int THREAD_SYS_INIT (void)
{
    int *stack;
    
    mtuMTP_errno = mtuMTP_NOINFO;      /* initialize error number  */
    
    Running = (THREAD_t) malloc(sizeof(TCB)); /* dummy running thrd*/
    if (Running == NULL)
    return mtu_ERROR;
    stack = (int *) malloc(64);        /* 64 bytes for stack       */
    if (stack == NULL) return mtu_ERROR;
    Running->Name = mtu_MAIN;          /* dummy = main()          */
    Running->StackBottom = stack;      /* StackBottom=dummy stack  */
    
    ReadyQ.Head = ReadyQ.Tail = NULL;  /* initialize ready and     */
    SuspendQ.Head = SuspendQ.Tail = NULL;   /* suspend queues      */
    SysTable.Head = SysTable.Tail = NULL;   /* initialize sys table*/
    
    SYSTEM_INITIALIZE = TRUE;
    
    return mtu_NORMAL;
}

/* ---------------------------------------------------------------- */
/* THREAD_WRAP:                                                     */
/*      This function wraps up the created thread and runs it as a  */
/* function.  The function to be run is indicated by "Running".     */
/*----------------------------------------------------------------- */

static volatile void THREAD_WRAP (void)
{
    (*Running->Entry) (Running->Argc, Running->Argv); /* run thread*/
    THREAD_EXIT();                     /* if user did not call     */
    /* THREAD_EXIT, do it here  */
}

/* ---------------------------------------------------------------- */
/* growsdown():                                                     */
/*   Checks the stack growing direction                             */
/*                                                                  */
/* Argument:                                                        */
/*   fromaddr   -- the beginning (reference) address                */
/* Return value:                                                    */
/*   0          -- stack direction is in increasing order of address*/
/*   nonzero    -- otherwise (decreasing order)                     */
/* Method:                                                          */
/*   "fromaddr" is from the caller and "toaddr" is a local variable */
/* Thus, if "fromaddr" > "toaddr", the stack growing direction is   */
/* going "upward" (i.e., new stack has a smaller address than those */
/* of previous stacks.                                              */
/*----------------------------------------------------------------- */

static int growsdown (void *fromaddr)
{
    int        toaddr;
    return fromaddr > (void *) &toaddr;
}

/* ---------------------------------------------------------------- */
/* THREAD_SCHEDULER():                                              */
/*   Schedules another thread to run from the ready queue.          */
/*                                                                  */
/* Return value:                                                    */
/*   mtu_ERROR      -- Failure                                      */
/*   mtu_NORMAL     -- Success                                      */
/* ---------------------------------------------------------------- */

static int THREAD_SCHEDULER (void)
{
    THREAD_t volatile Nextp;
    Nextp = (THREAD_t) THREAD_Remove (&ReadyQ); /* find a thread   */
    
    if (Nextp == NULL) {           /* if ready queue is empty  */
        mtuMTP_errno=mtuMTP_DEADLOCK; /* a deadlock may occur     */
        ShowDeadlock();
        exit(0);
        return mtu_ERROR;
    }
    if (Running==NULL) {           /* if running thread exited */
        Running = Nextp;          /* let the next run         */
        Nextp->Status = mtu_RUNNING;  /* its status = RUNNING     */
        RestoreEnvironment (Running->Environment); /* restore env*/
    }
    if ((Running != Nextp) && (SaveEnvironment(Running->Environment) == 0)) {
        /* else save running's env  */
        Running = Nextp;          /* let next thread run      */
        Nextp->Status = mtu_RUNNING;  /* its status = RUNNING     */
        RestoreEnvironment (Running->Environment); /* restore env */
    }
    
    return mtu_NORMAL;
}

/* ---------------------------------------------------------------- */
/* THREAD_READY():                                                  */
/*   Puts the input thread into the ready queue.                    */
/*                                                                  */
/* Argument:                                                        */
/*   THREAD_t ThreadID   -- the ID of the thread being put          */
/* Return value:                                                    */
/*   mtu_ERROR           -- Failure                                 */
/*   mtu_NORMAL          -- Success                                 */
/* ---------------------------------------------------------------- */

static int THREAD_READY (THREAD_t ThreadID)
{
    if ((ThreadID->Name < 0) || (ThreadID->Name > NextThreadName)) {
        mtuMTP_errno = mtuMTP_INVALIDID;   /* invalid ID          */
        return mtu_ERROR;
    }
    THREAD_Append (&ReadyQ, (void *)ThreadID); /* add to ready Q   */
    return mtu_NORMAL;
}
