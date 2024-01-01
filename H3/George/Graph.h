#pragma once

class Graph {
public:

	virtual int getNrCities() const = 0;
	virtual long long int distance(int i, int j) const = 0;
};