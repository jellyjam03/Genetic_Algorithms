#include "SA.h"
#define SA_ITERATIONS 100
#define LOW_TEMPERATURE 1

long long int SimulatedAnnealing(const Graph* G) {
	int t = 1, iterations = 0;
	Individual vc(G->getNrCities());
	long double T = 10000.0;
	vc.updateFVal(G);

	do {
		do {
			Improve(G, vc, T);
			iterations++;
		} while (iterations < SA_ITERATIONS * t);
		t++;
		cout << vc.fVal << '\n';
		UpdateTemperature(T);
	} while (T > LOW_TEMPERATURE);

	return vc.fVal;
}

void Improve(const Graph* G, Individual& vc, const long double T) {
	uniform_int_distribution<> neighbours{ 0, (int)vc.permutation.size() - 1 };
	int randPoz = neighbours(mt), prev, prevFVal;

	prevFVal = vc.fVal;
	prev = vc.permutation[randPoz];
	if (coin(mt) == 0)
		vc.permutation[randPoz] = (vc.permutation[randPoz] + 1) % (vc.permutation.size() + 1 - randPoz) + (vc.permutation[randPoz] + 1) / (vc.permutation.size() + 1 - randPoz);
	else
		if (vc.permutation[randPoz] == 1)
			vc.permutation[randPoz] = vc.permutation.size() + 1 - randPoz;
		else
			vc.permutation[randPoz]--;
	vc.updateFVal(G);

	if (vc.fVal < prevFVal)
		return;

	if (distribution(mt) < exp(-abs(vc.fVal - prevFVal) / T))
		return;

	vc.permutation[randPoz] = prev;
	vc.fVal = prevFVal;
}

void UpdateTemperature(long double& T) {
	T = T * 0.9995;
}