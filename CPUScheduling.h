#ifndef __CPUScheduling_h__
#define __CPUScheduling_h__
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "helper.h"

/* Will be the First Come First Serve CPU scheduling algorithm */
std::vector<double> FCFS(std::vector<Process> processes, int context_time);

/* Will be the Shortest Job First CPU scheduling algorithm */
std::vector<double> SJF(std::vector<Process> processes, int context_time);

/* Will be the Shortest Remaining Time CPU scheduling algorithm */
std::vector<double> SRT(std::vector<Process> processes, int context_time);

/* Will be the Round Robin CPU scheduling algorithm */
void RR();

/* Will be used to override the > operator*/
bool operator >(const Process proc_one, const Process proc_two);

#endif