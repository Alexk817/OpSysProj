#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <map>
#include "process.h"
#include "FCFS.h"
#include "SJF.h"
#include "SRT.h"
#include "RR.h"

std::vector<process> getTimes(int num_processes, int seed, double lambda, int upper_bound){
  srand48(seed);
  double r,x;
  std::vector<process> processes;
  for(int i=0; i<num_processes;i++){
    process temp;
    temp.name = 'A' + i;
    r = drand48();
    x = -log( r ) / lambda;
    while(x > upper_bound){
      r = drand48();
      x = -log( r ) / lambda;  
    }
    temp.arrival = floor(x);
    r = drand48();
    int num_bursts = floor(r * 100);
    std::vector<std::pair<int, int>> burst_times;
    for(int j=0; j<num_bursts;j++){
      std::pair<int, int> temp_pair;
      r = drand48();
      x = -log( r ) / lambda;
      while(x > upper_bound){
        r = drand48();
        x = -log( r ) / lambda;  
      }
      temp_pair.first = ceil(x);
      r = drand48();
      x = -log( r ) / lambda;
      while(x > upper_bound){
        r = drand48();
        x = -log( r ) / lambda;  
      }
      temp_pair.second = ceil(x);
      burst_times.push_back(temp_pair);
    }
    temp.CPU_bursts = burst_times;
    processes.push_back(temp);
  }

  return processes;
}

int main(int argc, char const *argv[]) {
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
	int alpha = atoi(argv[6]);
	int time_slice = atoi(argv[7]);
	std::string rr_add = "END";
	if (argc == 9) {
		rr_add = argv[8];
	}

  std::vector<process> test = getTimes(2, 5, .001, 3000);
  std::cout << test[0].name << ": " << test[0].arrival << "\n";
  std::cout << test[1].name << ": " << test[1].arrival << "\n";

	return EXIT_SUCCESS;
}