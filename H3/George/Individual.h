#pragma once
#include "Graph.h"
#include <vector>
#include <random>

using namespace std;

extern random_device rd;
extern seed_seq sq;
extern mt19937 mt;
extern uniform_int_distribution<> coin;
extern uniform_int_distribution<int> towns_distribution;
extern uniform_int_distribution<> distances;
extern uniform_int_distribution<> plusMinus;
extern uniform_real_distribution<long double> distribution;

class Individual {
public:
	vector<int>	permutation;
	long long int fVal;
	long double px, fitness;

	Individual() : fVal(0), px(0), fitness(0) {}
	Individual(const int size);
	//void mutate();
	void updateFVal(const Graph* G);
	void mutate(const int poz, const int direction);
};