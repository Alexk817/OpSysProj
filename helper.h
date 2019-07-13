#ifndef __helper_h__
#define __helper_h__
#include "process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>

// The previous guess the alpha constant and the actual time in the last round
int tauGuess(int lastTau, float alpha, int lastTime);

// Printing for the CPU sceduling algorithms
void printEvent(int time, std::string detail, std::vector<Process*> que);

// Calculate average CPU burst time
double calcAvgCPUBurst(std::vector<Process> &processes);

#endif