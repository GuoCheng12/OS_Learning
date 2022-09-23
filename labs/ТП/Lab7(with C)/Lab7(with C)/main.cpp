#include <iostream>
#include <stdlib.h>

#include "smoker.h"

int main(int argc, char *argv[])
{
    Smokerthread SmokerTobacco(0);  // smoker thread with "tobacco"
    Smokerthread SmokerMatch(1);    // smoker thread with "match"
    Smokerthread SmokerPaper(2);    // smoker thread with "paper"
    int Iterations;
    
    if (argc != 2) {
        cout << "Use " << argv[0] << " #-of-iterations of agent." << endl;
        exit(0);
    }
    Iterations = abs(atoi(argv[1]));
    
    // create the agent thread and fire it up
    Agentthread Agent("Agent", Iterations);
    Agent.Begin();
    
    // start smoker threads
    SmokerTobacco.Begin();
    SmokerMatch.Begin();
    SmokerPaper.Begin();
    
    // wait for all child threads
    Agent.Join();
    cout << "Game is over." << endl;
    Exit();
    
    return 0;
}

