#pragma once
#include "Individual.h"
#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <numeric>

#define POP_SIZE 200
#define GENETIC_ITERATIONS 10

bool fitnessBased(const Individual& a, const Individual& b);
bool fValBased(const Individual& a, const Individual& b);
bool pxBased(const Individual& a, const Individual& b);

long long int geneticAlgorithm(const Graph *G, const long double pm, const long double pm2, const long double px, const long double pElit, const int nrPoints);
void evaluate(vector<Individual>& pop, const Graph *G);
int getNextIndex(const Individual& x, int poz);
void selectGreedy(vector<Individual>& pop, const long double pElit);
void mutate(vector<Individual>& pop, const long double pm, const long double pm2);
void cross(vector<Individual>& pop, const long double px, const int nrPoints, const long double pm, const long double pm2);
void crossMethod(const int nrPoints, vector<Individual>& children, const Individual& parent1, const Individual& parent2, const long double pm, const long double pm2);

void selectWheelOfFortune(vector<Individual>& pop, const long double pElit);
void crossFitnessBased(vector<Individual>& pop, const long double px, const int nrPoints, const long double pm, const long double pm2);
void massExtintion(vector<Individual>& pop, const Graph* G, const long double pElit);
long double fitnessFunction(long long int fVal);
void mutateLargerNeighbourhood(vector<Individual>& pop, const long double pm, const long double pm2);