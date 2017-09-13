#ifndef _POPULATION_H 
#define _POPULATION_H
#include "Red.h"
#include <iostream>

using namespace std;

class Population
{
	int mutation_rate; //in %
public:
	int population_size;
	Red* individus;

	Population(int* structure, int size);
	void inputs (double *in);
	void evaluate(double** in, double** expected_result);
};
#endif
