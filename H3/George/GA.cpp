#include "GA.h"

void evaluate(vector<Individual>& pop, const Graph* G) {
	int i;
	long double sum = 0;

	for (i = 0; i < pop.size(); i++) {
		pop[i].updateFVal(G);
		sum += fitnessFunction(pop[i].fVal);
	}

	for (i = 0; i < pop.size(); i++)
		pop[i].fitness = fitnessFunction(pop[i].fVal) / sum;
}

int getNextIndex(const Individual& x, int poz) {
	int rez = x.permutation[poz], i;

	for (i = 0; i < poz; i++)
		if (x.permutation[i] <= x.permutation[poz])
			rez++;
	return rez;
}

long long int geneticAlgorithm(const Graph* G, const long double pm, const long double pm2, const long double px, const long double pElit, const int nrPoints) {
	long long int fValMin = 0, generation = 0, i, reignLen = 0;
	vector<Individual> pop;

	for (i = 0; i < POP_SIZE; i++)
		pop.push_back(Individual(G->getNrCities()));
	evaluate(pop, G);
	fValMin = pop[0].fVal;


	while (generation < GENETIC_ITERATIONS) {
		selectWheelOfFortune(pop, pElit);
		cout << generation << ' ' << fValMin << ' ' << pop[0].fVal << '\n';

		if (fValMin > pop[0].fVal) {
			fValMin = pop[0].fVal;
			reignLen = 1;
		}
		else
			reignLen++;

		/*if (reignLen > 100) 
			if (generation <= GENETIC_ITERATIONS/2) {
				reignLen = 0;
				massExtintion(pop, G, pElit);
			}*/

		mutate(pop, pm, pm2);
		cross(pop, px, nrPoints, pm, pm2);
		evaluate(pop, G);
		generation++;
	}
	return fValMin;
}

void selectGreedy(vector<Individual>& pop, const long double pElit) {
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

bool fitnessBased(const Individual& a, const Individual& b) {
	return a.fitness > b.fitness;
}

void mutate(vector<Individual>& pop, const long double pm, const long double pm2) {
	int i, j;
	long double mean = 0, p;

	for (i = 0; i < pop.size(); i++)
		mean += pop[i].fVal;
	mean /= pop.size();

	for (i = 0; i < pop.size(); i++) {
		if (pop[i].fVal <= mean || pop.size() == 2) 
			p = pm;
		else
			p = pm2;
		for (j = 0; j < pop[i].permutation.size(); j++)
			if (distribution(mt) <= p)
				if (coin(mt) == 0)
					pop[i].permutation[j] = (pop[i].permutation[j] + 1) % (pop[i].permutation.size() + 1 - j) + (pop[i].permutation[j] + 1) / (pop[i].permutation.size() + 1 - j);
				else
					if (pop[i].permutation[j] == 1)
						pop[i].permutation[j] = pop[i].permutation.size() + 1 - j;
					else
						pop[i].permutation[j]--;
	}
}

void cross(vector<Individual>& pop, const long double px, const int nrPoints, const long double pm, const long double pm2) {
	int i, j;
	vector<Individual> children;

	for (i = 0; i < pop.size(); i++)
		pop[i].px = distribution(mt);

	sort(pop.begin(), pop.end(), pxBased);
	for (i = 0; i < POP_SIZE; i+=2)
		if (pop[i].px < px && i + 1 < POP_SIZE)
			if (pop[i + 1].px < px || coin(mt) == 0) {
				crossMethod(nrPoints, children, pop[i], pop[i + 1], pm, pm2);
				for (j = 0; j < children.size(); j++)
					pop.push_back(children[j]);
				children.clear();
			}
			else
				break;
}

void crossMethod(const int nrPoints, vector<Individual>& children, const Individual& parent1, const Individual& parent2, const long double pm, const long double pm2) {
	Individual child1(parent1.permutation.size() + 1), child2(parent1.permutation.size() + 1);
	vector<Individual> aux;
	int i, currentIndex = 0;
	bool flip = coin(mt);

	if (nrPoints < 1 || parent1.permutation.size() < nrPoints)
		return;

	vector<int> indeces(parent1.permutation.size());
	iota(indeces.begin(), indeces.end(), 0);
	random_shuffle(indeces.begin(), indeces.end());

	vector<int> Points;
	for (i = 0; i < nrPoints; i++)
		Points.push_back(indeces[i]);
	Points.push_back(parent1.permutation.size() + 1);
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
	mutate(aux, pm, pm2);

	children.push_back(child1);
	children.push_back(child2);
	children.push_back(aux[0]);
	children.push_back(aux[1]);
}

void crossFitnessBased(vector<Individual>& pop, const long double px, const int nrPoints, const long double pm, const long double pm2) {
	int i, j;
	vector<Individual> children;

	sort(pop.begin(), pop.end(), fValBased);
	for (i = 0; i < POP_SIZE; i += 2)
		if (pop[i].px < px && i + 1 < POP_SIZE)
			if (pop[i + 1].px < px || coin(mt) == 0) {
				crossMethod(nrPoints, children, pop[i], pop[i + 1], pm, pm2);
				for (j = 0; j < children.size(); j++)
					pop.push_back(children[j]);
				children.clear();
			}
			else
				break;
}

void massExtintion(vector<Individual>& pop, const Graph* G, const long double pElit) {
	cout << "Meteor struck!\n";
	int i, srvCount = pop.size() * pElit;
	for (i = pop.size() - 1; i >= srvCount; i--)
		pop.erase(pop.begin() + i);

	while (pop.size() < POP_SIZE) {
		pop.push_back(Individual(G->getNrCities()));
		pop[pop.size() - 1].updateFVal(G);
	}
}

long double fitnessFunction(long long int fVal) {
	return 1.0 / pow(fVal, 2);
}

void selectWheelOfFortune(vector<Individual>& pop, const long double pElit) {
	int i, j, nrElit = 0;
	long double p, totalFitness = 0, sumFitness = 0;
	bool found;
	vector<Individual> newPop;

	for (i = 0; i < pop.size(); i++) {
		totalFitness += pop[i].fitness;
	}
	sort(pop.begin(), pop.end(), fitnessBased);

	//we apply elitism and select the first pElit*pop.size() individuals to live in the next generation
	nrElit = min((int)(pElit * pop.size()), (int)POP_SIZE);
	for (i = 0; i < nrElit; i++)
		newPop.push_back(pop[i]);
	newPop.push_back(pop[pop.size() - 1]); nrElit++;

	for (i = 0, sumFitness = 0; i < POP_SIZE - nrElit; i++) {//we select POP_SIZE individuals for the next generation
		p = distribution(mt);
		for (j = 0, sumFitness = 0, found = false; j < pop.size() && !found; j++) {//we search to see who won the wheel
			sumFitness += pop[i].fitness;
			if (sumFitness / totalFitness > p) {
				newPop.push_back(pop[j]);
				found = 1;
			}
		}
		if (!found) {
			newPop.push_back(pop[pop.size() - 1]);
		}
	}

	pop.clear();
	for (i = 0; i < newPop.size(); i++) {
		pop.push_back(newPop[i]);
	}
	sort(pop.begin(), pop.end(), fitnessBased);
}

void mutateLargerNeighbourhood(vector<Individual>& pop, const long double pm, const long double pm2) {
	int i, j, k, dist = 0;
	long double mean = 0, p;
	vector<int>indeces;

	for (i = 0; i < pop.size(); i++)
		mean += pop[i].fVal;
	mean /= pop.size();

	/*for (i = 0; i < 200; i++)
	{
		for (j = 0; j < 2; j++)
			indeces.push_back(i);
		cout << indeces.capacity() << ' ' << indeces.size() << '\n';
	}*/

	for (i = 0; i < pop[i].permutation.size() - 200; i++)
	{
		for (j = 0; j < 2; j++)
			indeces.push_back(i);
		cout << indeces.capacity() << ' ' << indeces.size() << '\n';
	}

	for (i = 0; i < pop.size(); i++) {
		if (pop[i].fVal < mean || pop.size() == 2)
			p = pm;
		else
			p = pm2;
		for (j = 0; j < pop[i].permutation.size(); j++)
			if (distribution(mt) < p)	{
				random_shuffle(indeces.begin(), indeces.end());
				dist = distances(mt);
				for (k = 0; k < dist; k++) {
					pop[i].mutate(indeces[k], plusMinus(mt));
				}
			}
	}			
}