#pragma once
#include "Common.h"
#define ITERATIONS_ANN 100

using namespace std;

typedef long double (*funcPointer)(const point& x);

long double simulatedAnnealing(const int& dimensions, funcPointer function, const long double a, const long double b, const long double epsilon);
long double UpdateTemperature(const long double temperature, const int& iterr);
void SelectRandomNeighbour(point& x, long double& fVal, funcPointer function, const long double T, const long double a, const long double b);
long double modul(const long double x);
void PrintAnneal(const int sampleSize, const int dimensions, funcPointer function, const char* functionName, const long double a, const long double b, const long double epsilon);
