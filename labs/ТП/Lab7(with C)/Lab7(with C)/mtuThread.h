#ifndef _MTP_H
#define _MTP_H

#if !defined(_SETJMP) && !defined(_SETJMP_INCLUDED) && !defined(_SETJMP_H)
#include <setjmp.h>
#define SaveEnvironment(x)      setjmp(x)
#define RestoreEnvironment(x)   longjmp(x, 1)
typedef jmp_buf Env;
#endif

#define   MAX_THREADS           200     /* max # of threads         */
#define   MAX_MUTEXES           50      /* max # of mutex locks     */
#define   MAX_SEMAPHORES        100     /* max # of semaphores      */

#define   THREAD_SIZE           16384   /* thread stack size        */

#define   mtu_RUNNING           100     /* thread states            */
#define   mtu_READY             101
#define   mtu_WAITING           102
#define   mtu_TERMINATED        103
#define   mtu_SUSPENDED         104
#define   mtu_JOINING           105

#define   THREAD_SUSPENDED      104
#define   THREAD_NORMAL         1

#define   mtu_MAIN              0       /* main thread ID           */
#define   mtu_INVALID           -1
#define   mtu_VALID             27197   /* a magic # for validity   */
#define   mtu_NOBODY            0

#define   mtu_ERROR             -1
#define   mtu_NORMAL            0

typedef double StackAlign;

typedef struct TCB_NODE *TCB_ptr;
typedef TCB_ptr THREAD_t;

typedef struct Node_DS *Node;
typedef struct Node_DS {
    void *Content;
    Node Next;
} NODE;

typedef struct Queue_DS {
    Node Head;
    Node Tail;
} Queue;

typedef struct TCB_NODE {           /* thread control block     */
    int         Name;          /* thread ID                */
    int         Status;            /* thread state             */
    Env         Environment;           /* processor context area   */
    void        *StackBottom;          /* bottom of stack attached */
    int         Size;          /* stack size               */
    void        (*Entry)(int, char**); /* entry point (function)   */
    int         Argc;          /* # of arguments           */
    char        **Argv;            /* argument list            */
    Queue       JoinList;          /* joining list of threads  */
} TCB;

typedef struct MUTEX *MUTEX_t;

struct MUTEX {                  /* mutex lock structure     */
    int Magic;                 /* magic # for id. purpose  */
    THREAD_t Owner;            /* owner of this mutex lock */
    Queue WaitingQ;            /* mutex lock queue         */
};

#ifdef __cplusplus
extern "C" {
#endif
    MUTEX_t MUTEX_INIT(void);           /* create a mutex lock      */
    int     MUTEX_DESTROY(MUTEX_t);         /* destroy a mutex lock     */
    int     MUTEX_LOCK (MUTEX_t);           /* lock a mutex             */
    int     MUTEX_UNLOCK (MUTEX_t);         /* unlock a mutex           */
#ifdef __cplusplus
}
#endif

typedef struct SEMAPHORE *SEM_t;

struct SEMAPHORE {              /* semaphore structure      */
    int Magic;                 /* magic # for id. purpose  */
    int Count;                 /* semaphore counter        */
    Queue WaitingQ;            /* semaphore queue          */
};

#ifdef __cplusplus
extern "C" {
#endif
    SEM_t SEMAPHORE_INIT (int);             /* create and init a sem    */
    int   SEMAPHORE_DESTROY (SEM_t);        /* destroy a semaphore      */
    int   SEMAPHORE_SIGNAL (SEM_t);         /* signal a semaphore       */
    int   SEMAPHORE_WAIT (SEM_t);           /* wait on a semaphore      */
#ifdef __cplusplus
}
#endif

typedef struct CHANNEL *CHANNEL_t;

typedef struct CHANNEL_PACK *CHANNEL_PACKAGE;

struct CHANNEL_PACK {
    int  Size;
    void *Storage;
};

struct CHANNEL {
    char   *Name;               /* Name of the Channel      */
    Queue  Buffer;              /* Pointer to buffer        */
    int    Size;                /* Size of the buffer       */
    int    Type;                /* Blocking, Nonblocking    */
    int    Mode;                /* Manytomany, onetoone, etc*/
    SEM_t  Guard;               /* Blocking the Queue       */
    SEM_t  Filled;              /* not full semaphore       */
    SEM_t  Emptied;             /* not empty semaphore      */
    
    THREAD_t Sender;            /* For right now store as   */
    THREAD_t Receiver;          /* threads                  */
};

#ifdef __cplusplus
extern "C" {
#endif
    
    CHANNEL_t CHAN_INIT(const char*, int, int);
    int CHAN_SEND(CHANNEL_t, void*, int);
    int CHAN_RECV(CHANNEL_t, void**, int*);
    int CHAN_DESTROY(CHANNEL_t);
#ifdef __cplusplus
}
#endif

/* Only to implement Asychronous - ManyToMany Channel */

#define ASYNCHRONOUS    0
/*#define SYNCHRONOUS   1*/

/*#define ONETOONE      0*/
/*#define MANYTOONE     1*/
#define MANYTOMANY      2

#ifdef __cplusplus
extern "C" {
#endif
    
    THREAD_t THREAD_CREATE (void (*)(), int, int, int, char **); /* create   */
    int      THREAD_EXIT (void);            /* terminate a thread       */
    void     THREAD_YIELD (void);           /* yield the control        */
    int      THREAD_JOIN (THREAD_t ID);     /* join with another thread */
    int      THREAD_SUSPEND(THREAD_t ID);   /* suspend a thread         */
    int      THREAD_RESUME(THREAD_t ID);    /* resume a thread          */
    THREAD_t THREAD_SELF(void );
    
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _MTP_H */
