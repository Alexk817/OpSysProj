#include "CPUScheduling.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "Taufunc.h"

void printEvent(int time, std::string detail, std::vector<Process> que){
    std::cout << time << "ms: " << detail << " [Q";
    if (que.size()){
        for (int i = 0; i < que.size(); i++){
            std::cout << " " < que[i].name() ;
        }
    }
    else {
        std::cout << "<empty>";
    }
    std::cout << "]" << std::endl;
}

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
    printEvent(curr_time,"Simulator started for FCFS",ready_queue);

    // While there are still processes running
    while (processes.size()) {
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival == curr_time) {
                ready_queue.push_back(processes[i]);
                //if it just arrived print this message
                if (processes[i].burst_num == 0){
                    printEvent(curr_time,"Process "+process[i].name+" arrived; added to ready queue",ready_queue);
                }
                //otherwise its retruning from i/o so print this
                 if (processes[i].burst_num == 0){
                    printEvent(curr_time,"Process "+process[i].name+" completed I/O; added to ready queue",ready_queue);
                }
            }
        }
        // If there is not a current process being run
        if (!curr_process) {
            if (ready_queue.size()) {
                //take what we think is the next process 
                curr_process = &ready_queue[0];
                ready_queue.erase(ready_queue.begin());
                //
                //do the context switch time to load it in
                 for (int j = 0; j < context_time/2; j++) {
                    curr_time++;
                    for (int i = 0; i < processes.size(); i++) {
                        if (processes[i].arrival == curr_time) {
                            ready_queue.push_back(processes[i]);
                            //if it just arrived print this message
                            if (processes[i].burst_num == 0){
                                printEvent(curr_time,"Process "+process[i].name+" arrived; added to ready queue",ready_queue);
                            }
                            //otherwise its retruning from i/o so print this
                             if (processes[i].burst_num == 0){
                                printEvent(curr_time,"Process "+process[i].name+" completed I/O; added to ready queue",ready_queue);
                            }
                        }
                    }
                }
                //now we ready to go
                printEvent(curr_time,"Process "+(*curr_process).name+" started using the CPU for "+std::string((*curr_process).CPU_bursts[(*curr_process).burst_num].first)+"ms burst",ready_queue);
                
            }
        }
        // If the current process ends in this timeslot
        if (!(*curr_process).CPU_bursts[(*curr_process).burst_num].first--) {
            // TODO: Output for turnaround times, wait times

            // Process is complete since we are on last CPU burst
            if ((*curr_process).CPU_bursts[(*curr_process).burst_num].second < 0) {
                done_process.push_back(*curr_process);
                // remove element from processes vector
                printEvent(curr_time,"Process "+(*curr_process).name+" terminated",ready_queue);
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
                printEvent(curr_time,"Process "+(*curr_process).name+" completed a CPU burst; "+std::string((*curr_process).CPU_bursts.size()-(*curr_process).burst_num)+"ms burst",ready_queue);
                (*curr_process).arrival = curr_time + (*curr_process).CPU_bursts[(*curr_process).burst_num].second;
                 printEvent(curr_time,"Process "+(*curr_process).name+" switching out of CPU; will block on I/O until time "+std::string((*curr_process).arrival)+"ms",ready_queue);
                (*curr_process).burst_num++;
            }
             // In a context switch it out
           
            for (int j = 0; j < context_time/2; j++) {
                curr_time++;
                for (int i = 0; i < processes.size(); i++) {
                    if (processes[i].arrival == curr_time) {
                        ready_queue.push_back(processes[i]);
                        //if it just arrived print this message
                        if (processes[i].burst_num == 0){
                            printEvent(curr_time,"Process "+process[i].name+" arrived; added to ready queue",ready_queue);
                        }
                        //otherwise its retruning from i/o so print this
                         if (processes[i].burst_num == 0){
                            printEvent(curr_time,"Process "+process[i].name+" completed I/O; added to ready queue",ready_queue);
                        }
                    }
                }
            }
            
            if (ready_queue.size()) {
                //take what we think is the next process 
                curr_process = &ready_queue[0];
                ready_queue.erase(ready_queue.begin());
                //
                //do the context switch time to load it in
                 for (int j = 0; j < context_time/2; j++) {
                    curr_time++;
                    for (int i = 0; i < processes.size(); i++) {
                        if (processes[i].arrival == curr_time) {
                            ready_queue.push_back(processes[i]);
                            //if it just arrived print this message
                            if (processes[i].burst_num == 0){
                                printEvent(curr_time,"Process "+process[i].name+" arrived; added to ready queue",ready_queue);
                            }
                            //otherwise its retruning from i/o so print this
                             if (processes[i].burst_num == 0){
                                printEvent(curr_time,"Process "+process[i].name+" completed I/O; added to ready queue",ready_queue);
                            }
                        }
                    }
                }
                //now we ready to go
                
                
            }
        
            else {
                curr_process = NULL;
            }
        }
        curr_time++;
    }
    printEvent(curr_time,"Simulator ended for FCFS",ready_queue);
}