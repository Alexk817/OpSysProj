#include <vector>
#include <utility>
#include "Process.h"

Process::Process(char aName, std::vector<std::pair<int, int>> aCPUBursts) {
    name = aName;
    CPU_bursts = aCPUBursts;
}

char Process::getName() const {
    return name;
}

std::vector<std::pair<int, int>> Process::getCPUBursts() const {
    return CPU_bursts;
}