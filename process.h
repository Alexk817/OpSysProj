#ifndef __process_h__
#define __process_h__
#include <vector>
#include <utility>

struct process {
	std::string name;
	int arrival;
	std::vector<std::pair<int, int>> CPU_bursts;
} process;

#endif