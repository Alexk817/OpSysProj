#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <map>

void getTimes(int seed, double lambda){
  srand48(seed);
  arrival time = drand48();
  double x = -log( r ) / lambda;
  int num_bursts = drand48() % 100; 
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
	return EXIT_SUCCESS;
}