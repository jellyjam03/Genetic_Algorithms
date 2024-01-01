#include "Individual.h"

random_device rd;
seed_seq sq = { rd(), rd(), rd(), rd() };
mt19937 mt(sq);
uniform_int_distribution<> coin{ 0, 1 };
uniform_int_distribution<int> towns_distribution;
uniform_int_distribution<> distances{ 1, 2 };
uniform_int_distribution<> plusMinus{ -1, 1 };
uniform_real_distribution<long double> distribution(0.0, 1.0);

Individual::Individual(const int size) : fVal(0), px(0), fitness(0) {
	int i;
	for (i = 0; i < size - 1; i++) {
		towns_distribution.param(std::uniform_int_distribution<int>::param_type(1, size-i));
		permutation.push_back(towns_distribution(mt));
	}
}

void Individual::updateFVal(const Graph* G) {
	int previous, next, i;
	vector<int> indeces;

	fVal = 0;
	previous = permutation[0];
	for (i = 1; i <= permutation.size() + 1; i++)
		indeces.push_back(i);
	indeces.erase(indeces.begin() + permutation[0] - 1);
	for (i = 1; i < permutation.size(); i++) {
		next = indeces[permutation[i] - 1];
		fVal += G->distance(previous - 1, next - 1);
		previous = next;
		indeces.erase(indeces.begin() + permutation[i] - 1);
	}
	fVal += G->distance(previous - 1, indeces[0] - 1); //distanta catre ultimul oras
	fVal += G->distance(indeces[0] - 1, permutation[0] - 1); //distanta de la ultimul oras la primul
}

//void Individual::mutate() {
//	towns_distribution.param(std::uniform_int_distribution<int>::param_type(1, permutation.size() + 1));
//	int poz = towns_distribution(mt);
//
//	permutation[poz]++;
//	permutation[poz] = permutation[poz] % (permutation.size() - poz + 1);
//}

void Individual::mutate(const int poz, const int direction) {
	permutation[poz] += direction;
	if (permutation[poz] < 1) permutation[poz] = permutation.size() + 1 - poz;
	if (permutation[poz] > permutation.size() + 1 - poz) permutation[poz] = 1;
}