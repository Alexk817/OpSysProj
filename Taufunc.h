#ifndef __Taufunc_h__
#define __Taufunc_h__
#include "process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>

//The previos guess the alpha constant and the actual time in the last round
int tauGuess(int lastTau, float alpha, int lastTime){
	return std::ceil(alpha * lastTime + (1-alpha) * lastTau);
}


#endif