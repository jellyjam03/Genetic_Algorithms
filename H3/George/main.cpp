//#include <iostream>
#include <chrono>
#include "GA.h"
#include "ATT_Graph.h"
#include "LDR_Graph.h"
#include "EUC_2D_Graph.h"
#include "SA.h"

using namespace std;

ATT_Graph ATT_G;
LDR_Graph LDR_G;
EUC_2D_Graph EUC_2D_G;

int main() {
	char inputPath[50] = "";
	cin >> inputPath;

	ifstream fin(inputPath);
	//ATTReadInput(fin, ATT_G);
	LDRReadInput(fin, LDR_G);
	//EUC_2DReadInput(fin, EUC_2D_G);
	
	chrono::steady_clock timer;
	chrono::steady_clock::time_point begin, end;
	begin = timer.now();
	//cout << geneticAlgorithm(&ATT_G, 2.0 / ATT_G.towns.size(), 4.0 / ATT_G.towns.size(), 1, 0.5, 50) << '\n';
	cout << geneticAlgorithm(&LDR_G, 1.2 / (int)sqrt(LDR_G.towns.size() * 2), 4.0 / (int)sqrt(LDR_G.towns.size() * 2), 1, 0.15, 10) << '\n';
	//cout << geneticAlgorithm(&EUC_2D_G, 2.0 / EUC_2D_G.towns.size(), 4.0 / EUC_2D_G.towns.size(), 1, 0.5, 100) << '\n';
	//cout << SimulatedAnnealing(&EUC_2D_G) << '\n';
	//cout << SimulatedAnnealing(&ATT_G) << '\n';
	//cout << SimulatedAnnealing(&LDR_G) << '\n';
	end = timer.now();
	cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count();

	
	return 0;
}
