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

char Process::getName() const {
    return name;
}

int Process::getArrival() const {
	return arrival;
}

std::vector<std::pair<int, int> > Process::getCPUBursts() const {
    return CPU_bursts;
}

std::string Process::getStatus() const {
    return status;
}

void Process::setStatus(std::string newStatus) {
    status = newStatus;
}