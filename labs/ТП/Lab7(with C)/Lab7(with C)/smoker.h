
#include "ThreadClass.h"

#define NUM_OF_SMOKERS      3        // number of Smoker

class Smokerthread: public Thread
{
    public:
    Smokerthread(int Number);  // constructor
    private:
    void ThreadFunc();
    int No;
};

class Agentthread: public Thread
{
    public:
    Agentthread(char *ThreadName, int iter);  // constructor
    private:
    void ThreadFunc();
    int Iterations;
};
