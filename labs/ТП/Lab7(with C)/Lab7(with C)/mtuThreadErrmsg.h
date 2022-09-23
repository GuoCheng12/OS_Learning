const int mtuMTP_NOINFO                 = 0;
const int mtuMTP_NOTINQUEUE             = 1;
const int mtuMTP_DEADLOCK               = 2;
const int mtuMTP_INVALIDID              = 3;
const int mtuMTP_NOTREADY               = 4;
const int mtuMTP_NOTSUS                 = 5;
const int mtuMTP_LOCKBYOWNER            = 6;
const int mtuMTP_NOTOWNER               = 7;
const int mtuMTP_INVALMUTEX             = 8;
const int mtuMTP_INVALSEM               = 9;
const int mtuMTP_CHANNOTEMPTY           = 10;
const int mtuMTP_CHANNOTOWN             = 11;

static char *ErrorMessage[] = {
    " No further error message available, check perror.",
    " The thread is not in the specified queue.",
    " There is no thread in ready queue, possibly a deadlock happens.",
    " The thread specified by id does not exist.",
    " The thread cannot be suspended since it's not ready.",
    " The thread cannot be continued since it's not suspended.",
    " The mutex is already locked by this thread.",
    " This thread is not the owner of the mutex and cannot unlock it.",
    " The MUTEX_t for the mutex is invalid. ",
    " The SEM_t for the semaphore is invalid. ",
    " The Channel Buffer is not empty. ",
    " The thread running cannot use this channel. "
};
