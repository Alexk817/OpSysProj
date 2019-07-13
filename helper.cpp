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
        for (int i = 0; i < que.size(); i++){
            std::cout << " " << (*que[i]).name ;
        }
    }
    else {
        std::cout << " <empty>";
    }
    std::cout << "]" << std::endl;
}