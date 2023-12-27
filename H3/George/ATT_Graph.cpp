#include "ATT_Graph.h"

long long int ATT_Graph::distance(int i, int j) const {
	ATT_nod a, b;
	long long int xd, yd, tDist;
	long double realDist;

	a = towns[i];
	b = towns[j];

	xd = a.x - b.x;
	yd = a.y - b.y;
	realDist = sqrt((xd * xd + yd * yd) / 10.0);
	tDist = (int)realDist;
	if (realDist - tDist >= 0.5) 
		return tDist + 1;
	return tDist;
}
