#include <vector>
#include <utility>
#include "Process.h"

Process::Process(char aName, int aArrival, std::vector<std::pair<int, int>> aCPUBursts) {
    name = aName;
    arrival = aArrival;
    CPU_bursts = aCPUBursts;
}

char Process::getName() const {
    return name;
}

int Process::getArrival() const {
	return arrival;
}

std::vector<std::pair<int, int>> Process::getCPUBursts() const {
    return CPU_bursts;
}