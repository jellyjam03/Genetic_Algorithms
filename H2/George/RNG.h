#pragma once
#include <random>

using namespace std;

extern random_device rd;
extern seed_seq sq;
extern mt19937 mt;
extern uniform_int_distribution<> coin;
extern uniform_real_distribution<long double> distribution;

//random_device rd;
//seed_seq sq = { rd(), rd(), rd(), rd() };
//mt19937 mt(sq);
//uniform_int_distribution<> coin{ 0, 1 };
//uniform_real_distribution<long double> distribution(0.0, 1.0);