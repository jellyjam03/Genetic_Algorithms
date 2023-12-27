#include "GA.h"

void evaluate(vector<Individual>& pop, const Graph* G) {
	int i, j, previous = 0, next;
	vector<int> indeces;

	for (i = 0; i < pop.size(); i++) {
		pop[i].fVal = 0;
		previous = pop[i].permutation[0];
		for (j = 1; j <= pop[i].permutation.size() + 1; j++)
			indeces.push_back(j);
		indeces.erase(indeces.begin() + pop[i].permutation[0] - 1);
		for (j = 1; j < pop[i].permutation.size(); j++) {
			next = indeces[pop[i].permutation[j]- 1];
			pop[i].fVal += G->distance(previous-1, next-1);
			previous = next;
			indeces.erase(indeces.begin() + pop[i].permutation[j] - 1);
		}
		pop[i].fVal += G->distance(previous-1, indeces[0]-1); //distanta catre ultimul oras
		pop[i].fVal += G->distance(indeces[0]-1, pop[i].permutation[0]-1); //distanta de la ultimul oras la primul
	}
}

int getNextIndex(const Individual& x, int poz) {
	int rez = x.permutation[poz], i;

	for (i = 0; i < poz; i++)
		if (x.permutation[i] <= x.permutation[poz])
			rez++;
	return rez;
}

long long int geneticAlgorithm(const Graph* G, const int nrCities, const long double pm, const long double px, const long double pElit, const int nrPoints) {
	long long int fValMin = 0, generation = 0, i;
	vector<Individual> pop;

	for (i = 0; i < POP_SIZE; i++)
		pop.push_back(Individual(nrCities));
	evaluate(pop, G);
	fValMin = pop[0].fVal;

	while (generation < GENETIC_ITERATIONS) {
		cout << generation << '\n';
		select(pop);
		if (fValMin > pop[0].fVal)
			fValMin = pop[0].fVal;
		mutate(pop, pm);
		cross(pop, px, nrPoints, pm);
		evaluate(pop, G);
		generation++;
	}
	return fValMin;
}

void select(vector<Individual>& pop) {
	sort(pop.begin(), pop.end(), fValBased);
	for (int i = pop.size() - 1; i >= POP_SIZE; i--)
		pop.erase(pop.begin() + i);
}

bool fValBased(const Individual& a, const Individual& b) {
	return a.fVal < b.fVal;
}

bool pxBased(const Individual& a, const Individual& b) {
	return a.px < b.px;
}

void mutate(vector<Individual>& pop, const long double pm) {
	int i, j;
	for (i = 0; i < pop.size(); i++)
		for (j = 0; j < pop[i].permutation.size(); j++)
			if (distribution(mt) <= pm)
				pop[i].permutation[j] = (pop[i].permutation[j] + 1) % (pop[i].permutation.size() + 1 - j) + (pop[i].permutation[j] + 1) / (pop[i].permutation.size() + 1 - j);
}

void cross(vector<Individual>& pop, const long double px, const int nrPoints, const long double pm) {
	int i, j;
	vector<Individual> children;

	for (i = 0; i < pop.size(); i++)
		pop[i].px = distribution(mt);

	sort(pop.begin(), pop.end(), pxBased);
	for (i = 0; i < POP_SIZE; i+=2)
		if (pop[i].px < px && i + 1 < POP_SIZE)
			if (pop[i + 1].px < px || coin(mt) == 0) {
				crossMethod(nrPoints, children, pop[i], pop[i + 1], pm);
				for (j = 0; j < children.size(); j++)
					pop.push_back(children[j]);
				children.clear();
			}
			else
				break;
}

void crossMethod(const int nrPoints, vector<Individual>& children, const Individual& parent1, const Individual& parent2, const long double pm) {
	Individual child1(parent1.permutation.size() + 1), child2(parent1.permutation.size() + 1);
	vector<Individual> aux;
	int i, currentIndex = 0;
	bool flip = coin(mt);

	if (nrPoints < 1 || parent1.permutation.size() > nrPoints)
		return;

	vector<int> indeces(parent1.permutation.size());
	iota(indeces.begin(), indeces.end(), 0);
	random_shuffle(indeces.begin(), indeces.end());

	vector<int> Points;
	for (i = 0; i < nrPoints; i++)
		Points.push_back(indeces[i]);
	sort(Points.begin(), Points.end());

	for (i = 0; i < child1.permutation.size(); i++) {
		if (i > Points[currentIndex]) {
			currentIndex++;
			flip = coin(mt);
		}
		if (flip == 0 /*currentIndex % 2 == 0*/) { //dam sansa egala
			child1.permutation[i] = parent1.permutation[i];
			child2.permutation[i] = parent2.permutation[i];
		}
		else {
			child1.permutation[i] = parent2.permutation[i];
			child2.permutation[i] = parent1.permutation[i];
		}
	}

	aux.push_back(child1);
	aux.push_back(child2);
	mutate(aux, pm);

	children.push_back(child1);
	children.push_back(child2);
	children.push_back(aux[0]);
	children.push_back(aux[1]);
}