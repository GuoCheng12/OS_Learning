#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "mtuThreadErrmsg.h"            /* for mtuThread messages   */
#include "mtuThread.h"                  /* for mtuThread header     */

/* ---------------------------------------------------------------- */
/*                     static global variables                      */
/* ---------------------------------------------------------------- */

static Queue ReadyQ;                    /* the ready queue          */
static Queue SuspendQ;                  /* the suspended queue      */
static Queue SysTable;                  /* the table that records   */
/* waiting threads in the   */
/* system                   */
static THREAD_t Running;                /* the running thread       */

static int NextThreadName = 1;          /* The thread name indicates*/
/* if the thread id is valid*/

static int NextSemNumber = 1;            /* The next semaphore number*/

static int mtuMTP_errno;                /* the current error number */

#define TRUE 1
#define FALSE 0
static int SYSTEM_INITIALIZE = FALSE;  /* flag for initialize system*/

#include  "mtuThreadPrototypes.h"

#include  "mtuThreadQueue.c"            /* queue management functs  */
#include  "mtuThreadMessage.c"          /* message functions        */
#include  "mtuThreadCore.c"             /* the core of this         */
#include  "mtuThreadTop.c"              /* user callable functions  */

#include  "mtuThreadSEM.c"
