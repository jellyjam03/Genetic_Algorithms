#include "EUC_2D_Graph.h"

long long int EUC_2D_Graph::distance(int i, int j) const {
	Nod_2D a, b;
	long long int xd, yd, tDist;
	long double realDist;

	a = towns[i];
	b = towns[j];

	xd = a.x - b.x;
	yd = a.y - b.y;
	realDist = sqrt(xd * xd + yd * yd);
	tDist = (int)realDist;
	if (realDist - tDist >= 0.5)
		return tDist + 1;
	return tDist;
}

int EUC_2D_Graph::getNrCities() const {
	return towns.size();
}

void EUC_2DReadInput(std::ifstream& fin, EUC_2D_Graph& G) {
	Nod_2D aux;
	int i;

	char line[100] = "";

	while (strcmp(line, "NODE_COORD_SECTION") != 0) {
		fin.getline(line, 100);
	}

	fin >> i;
	do {
		fin >> aux.x >> aux.y;
		G.towns.push_back(aux);
		fin >> i;
		if (i == 0) break;
	} while (true);
}