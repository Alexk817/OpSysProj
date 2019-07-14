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

struct SRT_compareTimeleft
{
    bool operator()(Process *&a, Process *&b)
    {
        if ((*a).working_tau > (*b).working_tau)
        {
            return false;
        }
        else if ((*a).working_tau == (*b).working_tau)
        {
            return ((*a).name < (*b).name);
        }
        return true;
    }
};
// Function to preempt a process and add it to the end or beginning of the ready queue
// Will set curr_process to the next element
void SRT_preemptProcess(Process *&curr_process, std::vector<Process *> &ready_queue)
{
    
    ready_queue.push_back(curr_process);
    std::sort(ready_queue.begin(), ready_queue.end(), SRT_compareTimeleft());
    curr_process = NULL;
}

void SRT_addArived(std::vector<Process> &processes, std::vector<Process *> &ready_queue, int &curr_time, char *buff2, Process* &curr_process)
{
    for (unsigned int i = 0; i < processes.size(); i++)
    {

        if (processes[i].arrival == curr_time)
        {
            ready_queue.push_back(&processes[i]);
            std::sort(ready_queue.begin(), ready_queue.end(), SRT_compareTimeleft());

            //if it just arrived print this message
            if (processes[i].burst_num == 0)
            {
                sprintf(buff2, "%d", processes[i].tau);
                if (curr_process && processes[i].working_tau < (*curr_process).working_tau){
                	printEvent(curr_time, std::string("Process ") + processes[i].name + " (tau " + buff2 + "ms)" + " arrived; preempting "+ (*curr_process).name, ready_queue);
                	curr_process->preempted = true;
                }
                else {
                	printEvent(curr_time, std::string("Process ") + processes[i].name + " (tau " + buff2 + "ms)" + " arrived; added to ready queue", ready_queue);
                }
                
            }
            //otherwise its retruning from i/o so print this
            else
            {
                sprintf(buff2, "%d", processes[i].tau);
                if (curr_process && processes[i].working_tau < (*curr_process).working_tau){
                	printEvent(curr_time, std::string("Process ") + processes[i].name + " (tau " + buff2 + "ms)" + " completed I/O; preempting "+ (*curr_process).name, ready_queue);
                	curr_process->preempted = true;
                }
                else {
               		printEvent(curr_time, std::string("Process ") + processes[i].name + " (tau " + buff2 + "ms)" + " completed I/O; added to ready queue", ready_queue);
               	}
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
        std::sort(ready_queue.begin(), ready_queue.end(), SRT_compareTimeleft());

        //do the context switch time to load it in
        for (int j = 0; j < context_time / 2; j++)
        {
            incWaitTime(ready_queue);
            curr_time++;
            if (j == context_time / 2 - 1)
            {
                sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);
                sprintf(buff2, "%d", (*curr_process).tau);
                printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU with " + buff + "ms burst remaining", ready_queue);

            }
            SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
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
    int num_preempt = 0;
    printEvent(curr_time, "Simulator started for SRT", ready_queue);
    bool just_preempt = false;

    // While there are still processes running
    while (active_processes.size())
    {
    	just_preempt = false;
        // If there is not a current process being run
        if (!curr_process)
        {
            SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
            SRT_popQueifPossible(ready_queue, curr_process, curr_time, processes, context_time, buff, buff2);
        }
        else
        {
        	if (curr_process->preempted)
        	{
        		just_preempt = true;
        		(*curr_process).working_tau++;
        		//(*curr_process).CPU_bursts[(*curr_process).burst_num].first;
        		std::cerr << "New tau: " << (*ready_queue[0]).working_tau << " Old Tau: " << (*curr_process).working_tau << std::endl;
                (*curr_process).preempted = true;
                num_preempt++;
                // Preempt the process and add the next process in the queue with a context switch
                //SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                num_context_switch++;
                for (int j = 1; j < context_time / 2; j++)
                {
                    incWaitTime(ready_queue);
                    curr_time++;
                    SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                }
                SRT_preemptProcess(curr_process, ready_queue);
            
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
                        // if ((*curr_process).preempted)
                        // {
                            printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU with " + buff + "ms burst remaining", ready_queue);
                            (*curr_process).preempted = false;
                        // }
                        // else
                        // {
                        //     printEvent(curr_time, std::string("Process ") + (*curr_process).name + " started using the CPU for " + buff + "ms burst", ready_queue);
                        // }
                    }
                    SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                }

                 (*curr_process).working_tau++;
                 (*curr_process).CPU_bursts[(*curr_process).burst_num].first++;
        	}
            // If the current process ends in this timeslot
            (*curr_process).working_tau--;
            if (!--(*curr_process).CPU_bursts[(*curr_process).burst_num].first && !just_preempt)
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

                }
                // In a context switch it out
                SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                num_context_switch++;
                for (int j = 0; j < context_time / 2; j++)
                {
                    incWaitTime(ready_queue);
                    curr_time++;
                    SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                }
                if (ready_queue.size())
                {
                    //take what we think is the next process
                    curr_process = (ready_queue[0]);
                    ready_queue.erase(ready_queue.begin());
                    std::sort(ready_queue.begin(), ready_queue.end(), SRT_compareTimeleft());

                    //do the context switch time to load it in
                    for (int j = 0; j < context_time / 2; j++)
                    {
                        incWaitTime(ready_queue);
                        curr_time++;
                        if (j == context_time / 2 - 1)
                        {
                            sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);
                            sprintf(buff2, "%d", (*curr_process).tau);
                            //printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU for " + buff + "ms burst", ready_queue);
							// if ((*curr_process).preempted)
       //                      {
                                printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU with " + buff + "ms burst remaining", ready_queue);
                                (*curr_process).preempted = false;
                            // }
                            // else
                            // {
                               // printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU for " + buff + "ms burst", ready_queue);
                            // }
                        }
                        SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                    }
                }
                else
                {
                    curr_process = NULL;
                }
            }
             // Preempt if neccessary
            // preempt any process that is being performed and add it to the beginning or end of the ready queue
            // adding to beginning or end depends on rr_add
            else if (ready_queue.size() && SRT_compareTimeleft()(ready_queue[0],curr_process))
            {
                
               
                
                sprintf(buff, "%d", (*curr_process).CPU_bursts[(*curr_process).burst_num].first);
                //TODO FIND Correct Input
                printEvent(curr_time, std::string("Process ") + (*curr_process).name + " preempted with " + buff + "ms to go", ready_queue);
                std::cerr << "New tau: " << (*ready_queue[0]).working_tau << " Old Tau: " << (*curr_process).working_tau << std::endl;
                (*curr_process).preempted = true;
                num_preempt++;
                // Preempt the process and add the next process in the queue with a context switch
                SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                num_context_switch++;
                for (int j = 0; j < context_time / 2; j++)
                {
                    incWaitTime(ready_queue);
                    curr_time++;
                    SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                }
                SRT_preemptProcess(curr_process, ready_queue);
            
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
                        // if ((*curr_process).preempted)
                        // {
                            printEvent(curr_time, std::string("Process ") + (*curr_process).name + " (tau " + buff2 + "ms)" + " started using the CPU with " + buff + "ms burst remaining", ready_queue);
                            (*curr_process).preempted = false;
                        // }
                        // else
                        // {
                        //     printEvent(curr_time, std::string("Process ") + (*curr_process).name + " started using the CPU for " + buff + "ms burst", ready_queue);
                        // }
                    }
                    SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
                }
                    
                    // else
                    // {
                    //     curr_process = NULL;
                    // }
                
                // ready queue is empty
               // else
                //{
                //    printEvent(curr_time, std::string("Time slice expired; no preemption because ready queue is empty"), ready_queue);
                //}
            }
            else
            {
                SRT_addArived(processes, ready_queue, curr_time, buff2,curr_process);
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
    ret_vals.push_back(double(num_preempt));
    return ret_vals;
}