//#include <iostream>
#include <fstream>
#include "GA.h"
#include "ATT_Graph.h"
#include "LDR_Graph.h"

using namespace std;

ATT_Graph ATT_G;
LDR_Graph LDR_G;

void ATTReadInput(ifstream &fin, ATT_Graph& G);
void LDRReadInput(ifstream &fin, LDR_Graph& G);

int main() {
	char inputPath[30] = "";
	cin >> inputPath;

	ifstream fin(inputPath);
	//ATTReadInput(fin, ATT_G);
	LDRReadInput(fin, LDR_G);

	//cout << geneticAlgorithm(&ATT_G, ATT_G.towns.size(), 1.0 / LDR_G.towns.size(), 1, 0.1, 10);
	cout << geneticAlgorithm(&LDR_G, (int)sqrt(LDR_G.towns.size() * 2), 1.0 / (int)sqrt(LDR_G.towns.size() * 2), 1, 0.1, 10);
	
	return 0;
}

void ATTReadInput(ifstream& fin, ATT_Graph& G) {
	ATT_nod aux;
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

void LDRReadInput(ifstream& fin, LDR_Graph& G) {
	int i;

	char line[100] = "";

	while (strcmp(line, "EDGE_WEIGHT_SECTION") != 0) {
		fin.getline(line, 100);
	}

	fin >> i;
	do {
		G.towns.push_back(i);
	} while (fin>>i);
	i = 3;
}
