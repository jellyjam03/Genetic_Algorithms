#include "RNG.h"

random_device rd;
seed_seq sq = { rd(), rd(), rd(), rd() };
mt19937 mt(sq);
uniform_int_distribution<> coin{ 0, 1 };
uniform_real_distribution<long double> distribution(0.0, 1.0);