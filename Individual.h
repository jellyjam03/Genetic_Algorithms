#pragma once
#include <vector>
#include <random>

using namespace std;

extern random_device rd;
extern seed_seq sq;
extern mt19937 mt;
extern uniform_int_distribution<> coin;
extern uniform_int_distribution<int> towns_distribution;
extern uniform_real_distribution<long double> distribution;

class Individual {
public:
	vector<int>	permutation;
	long long int fVal;
	long double px;

	Individual(const int size);
	void mutate();
};