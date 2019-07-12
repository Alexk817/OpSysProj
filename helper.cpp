#include "helper.h"
#include "process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>

int tauGuess(int lastTau, float alpha, int lastTime){
	return std::ceil(alpha * lastTime + (1-alpha) * lastTau);
}