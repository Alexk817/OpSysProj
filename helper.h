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
void printEvent(int time, std::string detail, std::vector<Process *> que);

// Calculate average CPU burst time
double calcAvgCPUBurst(std::vector<Process> &processes);

// Increment wait times for processes in the vector
void incWaitTime(std::vector<Process *> &ready_queue);

// Check if processes have arrived
void addArived(std::vector<Process> &processes, std::vector<Process *> &ready_queue, int &curr_time);

// Pop the element from ready queue if available
void popQueifPossible(std::vector<Process *> &ready_queue, Process *&curr_process, int &curr_time, std::vector<Process> &processes, int &context_time, char *buff);

// Calculate the average of a vector
double calcAvgVal(std::vector<int> times);

#endif