void ShowLastErr(const char *prefix)
{
    if (mtuMTP_errno == mtuMTP_NOINFO)
    perror(prefix); /* if no error message available, use perror()*/
    else {
        if (strcmp(prefix, "") != 0)   /* if there is a prefix,   */
        printf("%s : ", prefix); /*     then print it       */
        fprintf(stderr, "%s\n",  ErrorMessage[mtuMTP_errno]);
    }
    mtuMTP_errno = mtuMTP_NOINFO;
}

/* ---------------------------------------------------------------- */
/* ShowDeadlock:                                                    */
/*   Displays a possible reason for a deadlock.                     */
/* ---------------------------------------------------------------- */

void ShowDeadlock()
{
    THREAD_t  thread;
    SEM_t     Sem;
    
    fprintf(stderr, "A deadlock has occurred "
            " because there is no ready thread.\n");
    while ((Sem=(SEM_t)THREAD_Remove(&SysTable)) != NULL)
    {
        while ((thread=(THREAD_t)THREAD_Remove(&(Sem->WaitingQ)))!= NULL)
        {
            fprintf(stderr, "\tThread id = %d is waiting on semaphore "
                    "or mutex\n", thread->Name);
            if (thread->JoinList.Head != NULL)
            {
                fprintf(stderr, "\tFollowing threads is joinning thread "
                        "with (id = %d):\n", thread->Name);
                while ((thread=(THREAD_t) THREAD_Remove(&(thread->JoinList)))!= NULL)
                fprintf(stderr, "\t\tThread id = %d\n", thread->Name);
            }
        }
    }
}
