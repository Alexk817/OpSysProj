#ifndef __Process_h__
#define __Process_h__
#include <vector>
#include <utility>

class Process {
public:
    Process(char aName, std::vector<std::pair<int, int>> aCPUBursts);
    char getName() const;
    std::vector<std::pair<int, int>> getCPUBursts() const;

private:
    char name;
    std::vector<std::pair<int, int>> CPU_bursts;
};

#endif