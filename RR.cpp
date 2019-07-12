#include "CPUScheduling.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "helper.h"

/* 
Round Robin utilizes a pre determined time slice to run CPU bursts
Will prempt if process is not done within the time slice
 */
void RR(std::vector<Process> processes, int context_time, int time_slice) {
    // Vector to act as the ready queue
    std::vector<Process> ready_queue;
    // Vector to hold processes that are completed
    std::vector<Process> done_process;
    

}