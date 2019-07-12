#include "CPUScheduling.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "helper.h"

void FCFS(std::vector<Process> processes, int context_time) {
    // Vector to act as the ready queue
    std::vector<Process> ready_queue;
    // Vector to hold processes that are completed
    std::vector<Process> done_process;
    // Vector to hold turnaround times
    std::vector<int> turn_times;
    // Vector to hold wait
    std::vector<int> wait_times;
    // May not work - LOOK HERE
    Process* curr_process = NULL;
    int curr_time = 0;
    // While there are still processes running
    while (processes.size()) {
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival == curr_time) {
                ready_queue.push_back(processes[i]);
            }
        }
        // If there is not a current process being run
        if (!curr_process) {
            if (ready_queue.size()) {
                curr_process = &ready_queue[0];
                ready_queue.erase(ready_queue.begin());
            }
        }
        // If the current process ends in this timeslot
        if (!(*curr_process).CPU_bursts[(*curr_process).burst_num].first--) {
            // TODO: Output for turnaround times, wait times

            // Process is complete since we are on last CPU burst
            if ((*curr_process).CPU_bursts[(*curr_process).burst_num].second < 0) {
                done_process.push_back(*curr_process);
                // remove element from processes vector
                for (int i = 0; i < processes.size(); i++) {
                    if (processes[i].name == (*curr_process).name) {
                        processes.erase(processes.begin()+i);
                        break;
                    }
                }
                // TODO: Output for finished process
            }
            // Process finished burst but has to go to IO
            else {
                // Process will rearrive at this new time
                (*curr_process).arrival = curr_time + (*curr_process).CPU_bursts[(*curr_process).burst_num].second;
                (*curr_process).burst_num++;
            }
             // In a context switch
            for (int j = 0; j < context_time; j++) {
                curr_time++;
                for (int i = 0; i < processes.size(); i++) {
                    if (processes[i].arrival == curr_time) {
                        ready_queue.push_back(processes[i]);
                    }
                }
            }
            if (ready_queue.size()) {
                curr_process = &ready_queue[0];
                ready_queue.erase(ready_queue.begin());
            }
            else {
                curr_process = NULL;
            }
        }
        curr_time++;
    }
}