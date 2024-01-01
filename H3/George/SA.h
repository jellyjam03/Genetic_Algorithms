#pragma once
#include "Graph.h"
#include "Individual.h"
#include <iostream>

long long int SimulatedAnnealing(const Graph* G);
void Improve(const Graph* G, Individual& vc, const long double T);
void UpdateTemperature(long double& T);

