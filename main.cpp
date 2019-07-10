#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

void pseudoRandom(){
	srand48( time( NULL ) );

                      /* uniform to exponential distribution: */
                      /*                                      */
  double min = 0;     /*       -ln(1-r)                       */
  double max = 0;     /*  x = ----------                      */
  double sum = 0;     /*        lambda                        */

  int iterations = 1000000;    /* <== make this number very large */

  for ( int i = 0 ; i < iterations ; i++ )
  {
    double lambda = 0.001;  /* average should be 1/lambda ==> 1000 */

    double r = drand48();   /* uniform dist [0.00,1.00) -- also check out random() */
    double x = -log( r ) / lambda;  /* log() is natural log */

    /* avoid values that are far down the "long tail" of the distribution */
    if ( x > 3000 ) { i--; continue; }

    if ( i < 20 ) printf( "x is %lf\n", x );
    sum += x;
    if ( i == 0 || x < min ) { min = x; }
    if ( i == 0 || x > max ) { max = x; }
  }

  double avg = sum / iterations;

  printf( "minimum value: %lf\n", min );
  printf( "maximum value: %lf\n", max );
  printf( "average value: %lf\n", avg );
  return;
}


int main(int argc, char const *argv[])
{
	if (argc != 8 && argc != 9) {
		std::cerr << "Invalid number of argument" << std::endl;
		return EXIT_FAILURE;
	}
	int seed = atoi(argv[1]);
	double lambda = atof(argv[2]);
	int upper_bound = atoi(argv[3]);
	int num_processes = atoi(argv[4]);
	int time_context_switch = atoi(argv[5]);
	int alpha = atoi(argv[6]);
	int time_slice = atoi(argv[7]);
	std::string rr_add;
	if (argc == 9) {
		rr_add = argv[8];
	}

	pseudoRandom();
	
	return EXIT_SUCCESS;
}