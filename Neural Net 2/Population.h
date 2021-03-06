#ifndef _POPULATION_H 
#define _POPULATION_H
#include "Red.h"
#include <iostream>

using namespace std;

class Population
{
	float mutation_rate; //in %
	Red *child; 
	double*mapedNetErrors;
public:
	int population_size;
	Red* individus;
	double* networkErrors;
	double* fitness;
	double* probability;

	Population(int* structure, int size);
	void inputs (double *in);
	void evaluate(int nSamples, double** in, double** expected_result);
	void print_results();
	void new_generation();
	double average_error();
	int getBestNetwork();

private:
	void calculate_probability();
	int get_parent();
	Red reproduce(int parent1, int parent2);
	void mapErrors();
};
#endif
