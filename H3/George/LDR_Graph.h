#pragma once
#include "Graph.h"
#include "Common.h"

struct LDR_Graph : public Graph {
	std::vector<long long int> towns;

	int getNrCities() const;
	long long int distance(int i, int j) const;
};

void LDRReadInput(std::ifstream& fin, LDR_Graph& G);