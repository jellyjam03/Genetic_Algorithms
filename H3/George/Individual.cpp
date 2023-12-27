#include "Individual.h"

random_device rd;
seed_seq sq = { rd(), rd(), rd(), rd() };
mt19937 mt(sq);
uniform_int_distribution<> coin{ 0, 1 };
uniform_int_distribution<int> towns_distribution;
uniform_real_distribution<long double> distribution(0.0, 1.0);

Individual::Individual(const int size) : fVal(0), px(0) {
	int i;
	for (i = 0; i < size - 1; i++) {
		towns_distribution.param(std::uniform_int_distribution<int>::param_type(1, size-i));
		permutation.push_back(towns_distribution(mt));
	}
}

void Individual::mutate() {
	towns_distribution.param(std::uniform_int_distribution<int>::param_type(1, permutation.size() + 1));
	int poz = towns_distribution(mt);

	permutation[poz]++;
	permutation[poz] = permutation[poz] % (permutation.size() - poz + 1);
}
