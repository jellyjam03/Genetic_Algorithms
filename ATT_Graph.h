#pragma once
#include "Graph.h"
#include <vector>
#include <math.h>

struct ATT_nod{
	long long int x, y;

	ATT_nod() : x(0), y(0) {}
	ATT_nod(int a, int b) : x(a), y(b) {}
};

struct ATT_Graph : public Graph {
	std::vector<ATT_nod> towns;

	long long int distance(int i, int j) const;
};