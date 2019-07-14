/*
Jack Cherella - cherj
Jason Durr - durrj2
Alexander Kim - kima11
*/

#include "CPUScheduling.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "helper.h"
#include <algorithm>

struct compareTimeleft
{
    bool operator()(Process *&a, Process *&b)
    {
        if ((*a).tau > (*b).tau)
        {
            return false;
        }
        else if ((*a).tau == (*b).tau)
        {
            return ((*a).name < (*b).name);
        }
        return true;
    }
};

void SRT_addArived(std::vector<Process> &processes, std::vector<Process *> &ready_queue, int &curr_time, char *buff2)
{
    for (unsigned int i = 0; i < processes.size(); i++)
    {

        if (processes[i].arrival == curr_time)
        {
            ready_queue.push_back(&processes[i]);
            std::sort(ready_queue.begin(), ready_queue.end(), compareTimeleft());

            //if it just arrived print this message
            if (processes[i].burst_num == 0)
            {
                sprintf(buff2, "%d", processes[i].tau);
                printEvent(curr_time, std::string("Process ") + processes[i].name + " (tau " + buff2 + "ms)" + " arrived; added to ready queue", ready_queue);
            }
            //otherwise its retruning from i/o so print this
            else
            {
                sprintf(buff2, "%d", processes[i].tau);
                printEvent(curr_time, std::string("Process ") + processes[i].name + " (tau " + buff2 + "ms)" + " completed I/O; added to ready queue", ready_queue);
            }
        }
    }
}

void SRT_popQueifPossible(std::vector<Process *> &ready_queue, Process *&curr_process, int &curr_time, std::vector<Process> &processes, int &context_time, char *buff, char *buff2)
{
    if (ready_queue.size())
    {
        //take what we think is the next process
        curr_process = (ready_queue[0]);
        ready_queue.erase(ready_queue.begin());
        std::sort(ready_queue.begin(), ready_queue.end(), compareTimeleft());

        //do the context switch time to load it in
        for (int j = 0; j < context_time / 2; j++)
        {
            incWaitTime(ready_queue);
            curr_time++;
            if (j == context_time / 2 - 1)
            {
                sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);
                sprintf(buff2, "%d", (*curr_process).tau);
                printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU for " + buff + "ms burst", ready_queue);
            }
            SRT_addArived(processes, ready_queue, curr_time, buff2);
        }
    }
}

std::vector<double> SRT(std::vector<Process> processes, int context_time)
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
    char *buff2 = (char *)calloc(100, sizeof(char));
    // Return vector for output to simout.txt
    // [average CPU burst time, average wait time, average turnaround time, num context switches, num of preemtions]
    std::vector<double> ret_vals;
    ret_vals.push_back(calcAvgCPUBurst(processes));
    int num_context_switch = 0;
    printEvent(curr_time, "Simulator started for SRT", ready_queue);

    // While there are still processes running
    while (active_processes.size())
    {

        // If there is not a current process being run
        if (!curr_process)
        {
            SRT_addArived(processes, ready_queue, curr_time, buff2);
            SRT_popQueifPossible(ready_queue, curr_process, curr_time, processes, context_time, buff, buff2);
        }
        else
        {
            // If the current process ends in this timeslot
            if (!--(*curr_process).CPU_bursts[(*curr_process).burst_num].first)
            {
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
                    sprintf(buff2, "%d", (*curr_process).tau);
                    //if there is only one left make it singular
                    if (buff[0] == '1' && buff[1] == '\0')
                    {
                        printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " completed a CPU burst; " + buff + " burst to go", ready_queue);
                    }
                    //else make it plural
                    else
                    {
                        printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " completed a CPU burst; " + buff + " bursts to go", ready_queue);
                    }

                    (*curr_process).arrival = curr_time + (*curr_process).CPU_bursts[(*curr_process).burst_num].second + context_time / 2;

                    (*curr_process).burst_num++;
                    (*curr_process).updateTau();
                    sprintf(buff2, "%d", (*curr_process).tau);
                    printEvent(curr_time, std::string("Recalculated tau = ") + buff2 + "ms for process " + (*curr_process).name, ready_queue);
                    sprintf(buff, "%d", (*curr_process).arrival);
                    printEvent(curr_time, std::string("Process ") + (*curr_process).name + " switching out of CPU; will block on I/O until time " + buff + "ms", ready_queue);
                    //on to the next burse set

                    //Recalculated tau = 1016ms for process A
                }
                // In a context switch it out
                SRT_addArived(processes, ready_queue, curr_time, buff2);
                num_context_switch++;
                for (int j = 0; j < context_time / 2; j++)
                {
                    incWaitTime(ready_queue);
                    curr_time++;
                    SRT_addArived(processes, ready_queue, curr_time, buff2);
                }
                if (ready_queue.size())
                {
                    //take what we think is the next process
                    curr_process = (ready_queue[0]);
                    ready_queue.erase(ready_queue.begin());
                    std::sort(ready_queue.begin(), ready_queue.end(), compareTimeleft());

                    //do the context switch time to load it in
                    for (int j = 0; j < context_time / 2; j++)
                    {
                        incWaitTime(ready_queue);
                        curr_time++;
                        if (j == context_time / 2 - 1)
                        {
                            sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);
                            sprintf(buff2, "%d", (*curr_process).tau);
                            printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU for " + buff + "ms burst", ready_queue);
                        }
                        SRT_addArived(processes, ready_queue, curr_time, buff2);
                    }
                }
                else
                {
                    curr_process = NULL;
                }
            }
            else
            {
                SRT_addArived(processes, ready_queue, curr_time, buff2);
            }
        }
        incWaitTime(ready_queue);
        curr_time++;
    }
    //decrease current time by one because it add one before but its not actually another tick
    printEvent(curr_time - 1, "Simulator ended for SRT", ready_queue);
    free(buff);
    free(buff2);

    // Add values to the ret_vals
    ret_vals.push_back(calcAvgVal(wait_times));
    ret_vals.push_back(calcAvgVal(turn_times));
    ret_vals.push_back(double(num_context_switch));
    ret_vals.push_back(0.0);
    return ret_vals;
}