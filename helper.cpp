/*
Jack Cherella - cherj
Jason Durr - durrj2
Alexander Kim - kima11
*/

#include "helper.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>

int tauGuess(int lastTau, float alpha, int lastTime)
{
    return std::ceil(alpha * lastTime + (1 - alpha) * lastTau);
}

void printEvent(int time, std::string detail, std::vector<Process *> que)
{
    // if (time <= 999 || detail.find("terminated") != std::string::npos || detail.find("ended") != std::string::npos)
    //  {
        
        std::cout << "time " << time << "ms: " << detail << " [Q";
        if (que.size())
        {
            for (unsigned int i = 0; i < que.size(); i++)
            {
                std::cout << " " << (*que[i]).name;
            }
        }
        else
        {
            std::cout << " <empty>";
        }
        std::cout << "]" << std::endl;
    // }
}

double calcAvgCPUBurst(std::vector<Process> &processes)
{
    double total_time = 0;
    int num_of_burst = 0;
    for (unsigned int i = 0; i < processes.size(); i++)
    {
        num_of_burst += processes[i].CPU_bursts.size();
        for (unsigned int j = 0; j < processes[i].CPU_bursts.size(); j++)
        {
            total_time += processes[i].CPU_bursts[j].first;
        }
    }
    return total_time / num_of_burst;
}

void incWaitTime(std::vector<Process *> &ready_queue)
{
    for (unsigned int i = 0; i < ready_queue.size(); i++)
    {
        (*ready_queue[i]).wait_time++;
    }
}

void addArived(std::vector<Process> &processes, std::vector<Process *> &ready_queue, int &curr_time)
{
    for (unsigned int i = 0; i < processes.size(); i++)
    {

        if (processes[i].arrival == curr_time)
        {
            ready_queue.push_back(&processes[i]);
            //if it just arrived print this message
            if (processes[i].burst_num == 0)
            {
                printEvent(curr_time, std::string("Process ") + processes[i].name + " arrived; added to ready queue", ready_queue);
            }
            //otherwise its retruning from i/o so print this
            else
            {
                printEvent(curr_time, std::string("Process ") + processes[i].name + " completed I/O; added to ready queue", ready_queue);
            }
        }
    }
}

void popQueifPossible(std::vector<Process *> &ready_queue, Process *&curr_process, int &curr_time, std::vector<Process> &processes, int &context_time, char *buff)
{
    if (ready_queue.size())
    {
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

double calcAvgVal(std::vector<int> times)
{
    double sum = 0;
    for (unsigned int i = 0; i < times.size(); i++)
    {
        sum += times[i];
    }
    return sum / times.size();
}