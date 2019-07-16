/*
Jack Cherella - cherj
Jason Durr - durrj2
Alexander Kim - kima11
*/

#include <vector>
#include <utility>
#include <stdlib.h>
#include "Process.h"
#include <string>
#include "helper.h"
#include <algorithm>

Process::Process(char aName, int aArrival, std::vector<std::pair<int, int> > aCPUBursts, int initTau, double tAlpha)
{
    name = aName;
    arrival = aArrival;
    end = -1;
    burst_num = 0;
    CPU_bursts = aCPUBursts;
    status = "READY";
    alpha = tAlpha;
    tau = initTau;
    SIO_tau = initTau;
    working_tau = tau;
    SIO_next_tau = tauGuess(tau, alpha, CPU_bursts[burst_num].second);
    next_tau = tauGuess(tau, alpha, CPU_bursts[burst_num].first);
    wait_time = 0;
    preempted = false;
}
void Process::updateTau()
{
    tau = next_tau;
    working_tau = tau;
    next_tau = tauGuess(tau, alpha, CPU_bursts[burst_num].first);
    SIO_next_tau = tauGuess(tau, alpha, CPU_bursts[burst_num].second);
}
