#include "process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>

//The previos guess the alpha constant and the actual time in the last round
int tauGuess(int lastTao, float alpha, int lastTime){
	return std::ceil(alpha * lastTao + (1-alpha) * lastTao);

}