#include "LDR_Graph.h"

long long int LDR_Graph::distance(int i, int j) const {
	i++; j++;
	if (i < j) std::swap(i, j);
	return towns[i*(i-1)/2 + j - 1];
}