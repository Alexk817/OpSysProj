#include "helper.h"
#include "process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>

int tauGuess(int lastTau, float alpha, int lastTime){
	return std::ceil(alpha * lastTime + (1-alpha) * lastTau);
}

void printEvent(int time, std::string detail, std::vector<Process*> que){
    std::cout << "time " << time << "ms: " << detail << " [Q";
    if (que.size()){
        for (unsigned int i = 0; i < que.size(); i++){
            std::cout << " " << (*que[i]).name ;
        }
    }
    else {
        std::cout << " <empty>";
    }
    std::cout << "]" << std::endl;
}

double calcAvgCPUBurst(std::vector<Process> &processes) {
	double total_time = 0;
	int num_of_burst = 0;
	for (unsigned int i = 0; i < processes.size(); i++) {
		num_of_burst += processes[i].CPU_bursts.size();
		for (unsigned int j = 0; j < processes[i].CPU_bursts.size(); j++) {
			total_time += processes[i].CPU_bursts[j].first;
		}
	}
	return total_time / num_of_burst;
}