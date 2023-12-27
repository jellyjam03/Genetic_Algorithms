#pragma once
#include "Graph.h"
#include <vector>

struct LDR_Graph : public Graph {
	std::vector<long long int> towns;

	long long int distance(int i, int j) const;
};
