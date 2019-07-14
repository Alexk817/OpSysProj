#include "CPUScheduling.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "helper.h"

// Function to preempt a process and add it to the end or beginning of the ready queue
// Will set curr_process to the next element
void preemptProcess(Process *&curr_process, std::vector<Process *> &ready_queue, std::string &rr_add)
{
    if (!rr_add.compare("BEGINNING"))
    {
        ready_queue.insert(ready_queue.begin(), curr_process);
    }
    else if (!rr_add.compare("END"))
    {
        ready_queue.push_back(curr_process);
    }
    curr_process = NULL;
}

void RR_popQueifPossible(std::vector<Process *> &ready_queue, Process *&curr_process, int &curr_time, std::vector<Process> &processes, int &context_time, char *buff, int &curr_time_slice)
{
    if (ready_queue.size())
    {
        std::cerr << curr_time << " " << curr_time_slice << std::endl;
        //take what we think is the next process
        curr_process = (ready_queue[0]);
        ready_queue.erase(ready_queue.begin());
        //do the context switch time to load it in
        for (int j = 0; j < context_time / 2; j++)
        {
            incWaitTime(ready_queue);
            curr_time++;
            if (j == context_time / 2 - 1)
            {
                sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);

                printEvent(curr_time, std::string("Process ") + (*curr_process).name + " started using the CPU for " + buff + "ms burst", ready_queue);
            }
            addArived(processes, ready_queue, curr_time);
        }
    }
}

std::vector<double> RR(std::vector<Process> processes, int context_time, int time_slice, std::string rr_add)
{
    // Vector to act as the ready queue
    std::vector<Process *> ready_queue;
    //Vector of processes still active
    std::vector<Process> active_processes = processes;
    // Vector to hold processes that are completed
    std::vector<Process> done_process;
    // Vector to hold turnaround times
    std::vector<int> turn_times;
    // Vector to hold wait
    std::vector<int> wait_times;
    // May not work - LOOK HERE
    Process *curr_process = NULL;
    int curr_time = 0;
    //buffer for converting ints to strings
    char *buff = (char *)calloc(100, sizeof(char));
    // Return vector for output to simout.txt
    // [average CPU burst time, average wait time, average turnaround time, num context switches, num of preemtions]
    std::vector<double> ret_vals;
    ret_vals.push_back(calcAvgCPUBurst(processes));
    int num_context_switch = 0;
    int num_preempt = 0;
    int curr_time_slice = 1;
    printEvent(curr_time, "Simulator started for RR", ready_queue);

    // While there are still processes running
    while (active_processes.size())
    {
        // If there is not a current process being run
        if (!curr_process)
        {
            addArived(processes, ready_queue, curr_time);
            RR_popQueifPossible(ready_queue, curr_process, curr_time, processes, context_time, buff, curr_time_slice);
        }
        else
        {
            // The time slice has been hit
            // preempt any process that is being performed and add it to the beginning or end of the ready queue
            // adding to beginning or end depends on rr_add
            if (curr_time_slice == time_slice)
            {
                curr_time_slice = 1;
                // Only preempt if there are other processes on the ready queue
                if (ready_queue.size())
                {
                    sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);
                    printEvent(curr_time, std::string("Time slice expired; process ") + (*curr_process).name + " preempted with " + buff + "ms to go", ready_queue);
                    preemptProcess(curr_process, ready_queue, rr_add);
                    num_preempt++;
                    // Preempt the process and add the next process in the queue with a context switch
                    addArived(processes, ready_queue, curr_time);
                    num_context_switch++;
                    for (int j = 0; j < context_time / 2; j++)
                    {
                        incWaitTime(ready_queue);
                        curr_time++;
                        addArived(processes, ready_queue, curr_time);
                    }
                    if (ready_queue.size())
                    {
                        //take what we think is the next process
                        curr_process = ready_queue[0];
                        ready_queue.erase(ready_queue.begin());

                        //do the context switch time to load it in
                        for (int j = 0; j < context_time / 2; j++)
                        {
                            incWaitTime(ready_queue);
                            curr_time++;
                            if (j == context_time / 2 - 1)
                            {
                                sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);

                                printEvent(curr_time, std::string("Process ") + (*curr_process).name + " started using the CPU for " + buff + "ms burst", ready_queue);
                            }
                            addArived(processes, ready_queue, curr_time);
                        }
                    }
                    else
                    {
                        curr_process = NULL;
                    }
                }
                // ready queue is empty
                else
                {
                    printEvent(curr_time, std::string("Time slice expired; no preemption because ready queue is empty"), ready_queue);
                }
            }
            // If the current process ends in this timeslot
            else if (!--(*curr_process).CPU_bursts[(*curr_process).burst_num].first)
            {
                curr_time_slice = 1;
                // add for turnaround times, wait times to vectors
                turn_times.push_back(curr_time - (*curr_process).arrival + (context_time / 2));
                wait_times.push_back((*curr_process).wait_time);
                (*curr_process).wait_time = 0;

                // Process is complete since we are on last CPU burst
                if ((*curr_process).CPU_bursts[(*curr_process).burst_num].second < 0)
                {
                    done_process.push_back(*curr_process);
                    // remove element from processes vector
                    printEvent(curr_time, std::string("Process ") + (*curr_process).name + " terminated", ready_queue);
                    for (unsigned int i = 0; i < active_processes.size(); i++)
                    {
                        if (active_processes[i].name == (*curr_process).name)
                        {
                            active_processes.erase(active_processes.begin() + i);
                            break;
                        }
                    }
                }
                // Process finished burst but has to go to IO
                else
                {
                    // Process will rearrive at this new time
                    sprintf(buff, "%ld", ((*curr_process).CPU_bursts.size() - (*curr_process).burst_num) - 1);
                    //if there is only one left make it singular
                    if (buff[0] == '1' && buff[1] == '\0')
                    {
                        printEvent(curr_time, std::string("Process ") + (*curr_process).name + " completed a CPU burst; " + buff + " burst to go", ready_queue);
                    }
                    //else make it plural
                    else
                    {
                        printEvent(curr_time, std::string("Process ") + (*curr_process).name + " completed a CPU burst; " + buff + " bursts to go", ready_queue);
                    }
                    (*curr_process).arrival = curr_time + (*curr_process).CPU_bursts[(*curr_process).burst_num].second + context_time / 2;

                    sprintf(buff, "%d", (*curr_process).arrival);
                    printEvent(curr_time, std::string("Process ") + (*curr_process).name + " switching out of CPU; will block on I/O until time " + buff + "ms", ready_queue);
                    //on to the next burse set
                    (*curr_process).burst_num++;
                }
                // In a context switch it out
                addArived(processes, ready_queue, curr_time);
                num_context_switch++;
                for (int j = 0; j < context_time / 2; j++)
                {
                    incWaitTime(ready_queue);
                    curr_time++;
                    addArived(processes, ready_queue, curr_time);
                }
                if (ready_queue.size())
                {
                    //take what we think is the next process
                    curr_process = ready_queue[0];
                    ready_queue.erase(ready_queue.begin());

                    //do the context switch time to load it in
                    for (int j = 0; j < context_time / 2; j++)
                    {
                        incWaitTime(ready_queue);
                        curr_time++;
                        if (j == context_time / 2 - 1)
                        {
                            sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);

                            printEvent(curr_time, std::string("Process ") + (*curr_process).name + " started using the CPU for " + buff + "ms burst", ready_queue);
                        }
                        addArived(processes, ready_queue, curr_time);
                    }
                }
                else
                {
                    curr_process = NULL;
                }
            }
            else
            {
                addArived(processes, ready_queue, curr_time);
                curr_time_slice++;
            }
        }
        incWaitTime(ready_queue);
        curr_time++;
    }
    //decrease current time by one because it add one before but its not actually another tick
    printEvent(curr_time - 1, "Simulator ended for RR", ready_queue);
    free(buff);

    // Add values to the ret_vals
    ret_vals.push_back(calcAvgVal(wait_times));
    ret_vals.push_back(calcAvgVal(turn_times));
    ret_vals.push_back(double(num_context_switch));
    ret_vals.push_back(double(num_preempt));
    return ret_vals;
}