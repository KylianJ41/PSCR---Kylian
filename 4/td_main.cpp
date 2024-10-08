#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include "td.h"

int main(int argc, const char **argv)
{
    int N = 3;
    if (argc > 1)
    {
        N = atoi(argv[1]);
    }
    // pour eviter des executions trop reproductibles, pose le seed
    ::srand(::time(nullptr));
    return pr::createAndWait(N);
}

// Q3 comme on a un wait dans work, created apparait avant finished.
// Sans ce wait on ne pourrait pas dire, ce pourrait être l'un ou l'autre
