#pragma once
#include "Graph.h"
#include "Nod_2D.h"
#include "Common.h"

struct EUC_2D_Graph : public Graph {
	std::vector<Nod_2D> towns;

	int getNrCities() const;
	long long int distance(int i, int j) const;
};

void EUC_2DReadInput(std::ifstream& fin, EUC_2D_Graph& G);