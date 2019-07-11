#ifndef __Process_h__
#define __Process_h__
#include <vector>
#include <utility>

class Process {
public:
    Process(char aName, int aArrival, std::vector<std::pair<int, int>> aCPUBursts);
    char getName() const;
    int getArrival() const;
    std::vector<std::pair<int, int>> getCPUBursts() const;

private:
    char name;
    int arrival;
    std::vector<std::pair<int, int>> CPU_bursts;
};

#endif