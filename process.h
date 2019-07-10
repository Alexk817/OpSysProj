#ifndef __process_h__
#define __process_h__
#include <vector>
#include <utility>

struct process {
	char name;
	int arrival;
	std::vector<std::pair<int, int>> CPU_bursts;
};

#endif