static int THREAD_SYS_INIT ();

static volatile void THREAD_WRAP (void);
static int growsdown (void *fromaddr);
static void THREAD_INIT (volatile TCB *volatile NewThread, void *StackPointer);
static int THREAD_SCHEDULER (void);
static int THREAD_READY (THREAD_t ThreadID);

void ShowLastErr(const char *prefix);
void ShowDeadlock();
