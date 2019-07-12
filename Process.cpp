#include <vector>
#include <utility>
#include <stdlib.h>
#include "Process.h"
#include <string>

Process::Process(char aName, int aArrival, std::vector<std::pair<int, int> > aCPUBursts,int initTau) {
    name = aName;
    arrival = aArrival;
    CPU_bursts = aCPUBursts;
    status = "READY";
    tau = initTau;
}
