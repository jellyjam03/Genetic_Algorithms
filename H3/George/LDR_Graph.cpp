#include "LDR_Graph.h"

long long int LDR_Graph::distance(int i, int j) const {
	i++; j++;
	if (i < j) std::swap(i, j);
	return towns[i*(i-1)/2 + j - 1];
}

int LDR_Graph::getNrCities() const {
	return (int)sqrt(towns.size() * 2);
}

void LDRReadInput(std::ifstream& fin, LDR_Graph& G) {
	int i;

	char line[100] = "";

	while (strcmp(line, "EDGE_WEIGHT_SECTION") != 0) {
		fin.getline(line, 100);
	}

	fin >> i;
	do {
		G.towns.push_back(i);
	} while (fin >> i);
	i = 3;
}