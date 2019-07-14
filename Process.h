#ifndef __Process_h__
#define __Process_h__
#include <vector>
#include <utility>
#include <stdlib.h>
#include <string>
#include <algorithm>

/* This class represents process that the CPU will run */
class Process
{
public:
    Process(char aName, int aArrival, std::vector<std::pair<int, int>> aCPUBursts, int initTau, double tAlpha);
    void updateTau();
    char name;
    int arrival;
    int end;
    int burst_num;
    std::vector<std::pair<int, int>> CPU_bursts;
    std::string status;
    int tau;
    int wait_time;
    int next_tau;
    double alpha;
    bool preempted;
};

#endif