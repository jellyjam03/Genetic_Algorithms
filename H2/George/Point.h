#pragma once
#include "RNG.h"
#define MAX_BIT_SIZE 1024
#define MAX_RL_SIZE 256

int log2Superior(const long double x);

class point {
public:
    bool bit_form[MAX_BIT_SIZE];
    long double real_form[MAX_RL_SIZE];
    int nrBiti, length, dim;

    point() : bit_form{ 0 }, real_form{ 0 }, nrBiti(0), length(0), dim(0) {}

    point(const long double a, const long double b, const long double epsilon, const int& dimensions);

    point(const point& x);

    void copy(const point& x);

    void UpdateReal(const long double a, const long double b);

    void BitFlip(const long double a, const long double b, const int poz);
};

void randomCandidate(point& vc, const long double a, const long double b, const long double epsilon, const int& dimensions);