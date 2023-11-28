#pragma once
#include "Common.h"
#define ITERATIONS 100

typedef long double (*funcPointer)(const point& x);
typedef long double (*improvePointer)(point& x, const long double fVal, const long double a, const long double b, funcPointer function);

long double BestImprovement(point& x, const long double fVal, const long double a, const long double b, funcPointer function);
long double WorstImprovement(point& x, const long double fVal, const long double a, const long double b, funcPointer function);
long double FirstImprovement(point& x, const long double fVal, const long double a, const long double b, funcPointer function);
long double hillClimbing(const int& dimensions, funcPointer function, const long double a, const long double b, const long double epsilon, improvePointer Improve);
void Print(const int sampleSize, const int dimensions, improvePointer Improve, const char* impName, funcPointer function, const char* fName, const long double a, const long double b, const long double epsilon);
