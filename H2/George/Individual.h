#pragma once
#include "Point.h"

class Individual {
public:
    point x;
    long double fitness, fVal, px;

    Individual() : x() {
        fitness = fVal = px = 0;
    }

    Individual(const long double a, const long double b, const long double epsilon, const int& dimensions) : x(a, b, epsilon, dimensions) {
        fitness = fVal = px = 0;
    }
};
