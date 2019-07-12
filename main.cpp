#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <map>
#include "Process.h"
#include "CPUScheduling.h"

/* 
  Generate a vector of processes that the CPU scheduling algorithms with use
  Each process with have a vector of CPU Bursts of size 1-100
  Each Burst will have a CPU Burst time and IO Burst time
*/
std::vector<Process> generateProcesses(int num_processes, int seed, double lambda, int upper_bound) {
  srand48(seed);
  double r,x;
  std::vector<Process> processes;
  char name;
  for(int i = 0; i < num_processes; i++) {
    name = 'A' + i;
    r = drand48();
    x = -log( r ) / lambda;
    while(x > upper_bound) {
      r = drand48();
      x = -log( r ) / lambda;  
    }
    int arrival = floor(x);
    r = drand48();
    int num_bursts = floor(r * 100);
    std::vector<std::pair<int, int> > burst_times;
    for(int j = 0; j < num_bursts; j++) {
      std::pair<int, int> temp_pair;
      r = drand48();
      x = -log( r ) / lambda;
      while(x > upper_bound) {
        r = drand48();
        x = -log( r ) / lambda;  
      }
      temp_pair.first = ceil(x);
      r = drand48();
      x = -log( r ) / lambda;
      while(x > upper_bound) {
        r = drand48();
        x = -log( r ) / lambda;  
      }
      temp_pair.second = ceil(x);
      burst_times.push_back(temp_pair);
    }
    Process temp(name, arrival, burst_times,ceil(1/lambda));
    processes.push_back(temp);
  }

  return processes;
}

int main(int argc, char const *argv[]) {
  // Reading in Arguments
	if (argc != 8 && argc != 9) {
		std::cerr << "Invalid number of argument" << std::endl;
		return EXIT_FAILURE;
	}
	int seed = atoi(argv[1]);
	double lambda = atof(argv[2]);
	int upper_bound = atoi(argv[3]);
	int num_processes = atoi(argv[4]);
	if (num_processes > 26) {
		std::cerr << "Number of processes must be less than or equal to 26" << std::endl;
		return EXIT_FAILURE;
	}
	int time_context_switch = atoi(argv[5]);
	double alpha = atof(argv[6]);
	int time_slice = atoi(argv[7]);
	std::string rr_add = "END";
	if (argc == 9) {
		if (std::string(argv[8]).compare( "BEGINNING") && std::string(argv[8]).compare( "END")){
			std::cerr << "ERROR: INVALID READY QUE OPTION\nCORRECT USAGE: [BEGINNING | END]" << std::endl;
		}
		rr_add = std::string(argv[8]);
	}
	
	return EXIT_SUCCESS;
}