#pragma once
#define _USE_MATH_DEFINES //for M_PI
#include <math.h>
#include "Point.h"

long double Dejong(const point& x);
long double Schwefel(const point& x);
long double Rastrigin(const point& x);
long double Michalewicz(const point& x);