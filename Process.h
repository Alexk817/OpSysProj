#ifndef __Process_h__
#define __Process_h__
#include <vector>
#include <utility>
#include <stdlib.h>
#include <string>

/* This class represents process that the CPU will run */
class Process {
public:
    Process(char aName, int aArrival, std::vector<std::pair<int, int> > aCPUBursts);
    char getName() const;
    int getArrival() const;
    std::vector<std::pair<int, int> > getCPUBursts() const;
    std::string getStatus() const;
    void setStatus(std::string newStatus);

private:
    char name;
    int arrival;
    std::vector<std::pair<int, int> > CPU_bursts;
    std::string status;
};

#endif