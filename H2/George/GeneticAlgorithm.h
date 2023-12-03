#pragma once
#include "Common.h"
#include "Individual.h"
#define POP_SIZE 200
#define GENETIC_ITERATIONS 2000


typedef long double (*funcPointer)(const point& x);
typedef long double (*fitnessPointer)(const point& x, funcPointer function);
typedef void (*selectPointer)(vector<Individual>& pop, const long double pElit);
typedef void (*mutatePointer)(vector<Individual>& pop, const long double pm, const long double a, const long double b);
typedef void (*crossPointer)(const int nrPoints, vector<Individual>& children, const Individual& parent1, const Individual& parent2, mutatePointer mutate, const long double pm, const long double a, const long double b, const long double epsilon, const int dimensions);
typedef void (*parentSelectionPointer)(vector<Individual>& pop, crossPointer crossMethod, mutatePointer mutate, const long double pm, const long double px, const int nrPoints, const long double a, const long double b, const long double epsilon, const int dimensions);
typedef void (*evaluatePointer)(vector<Individual>& pop, fitnessPointer fitness, funcPointer function);


long double geneticAlgorithm(const int dimensions, fitnessPointer fitness, funcPointer function, selectPointer select, mutatePointer mutate, parentSelectionPointer parentSelection, crossPointer crossMethod, evaluatePointer evaluate, const long double pm, const long double px, const long double pElit, const int nrPoints, const long double a, const long double b, const long double epsilon);
void evaluate(vector<Individual>& pop, fitnessPointer fitness, funcPointer function);
void unifromMutate(vector<Individual>& pop, const long double pm, const long double a, const long double b);
void selectWheelOfFortune(vector<Individual>& pop, const long double pElit);
void selectGreedy(vector<Individual>& pop, const long double pElit);
void sortParentSelection(vector<Individual>& pop, crossPointer crossMethod, mutatePointer mutate, const long double pm, const long double px, const int nrPoints, const long double a, const long double b, const long double epsilon, const int dimensions);
void fitnessParentSelection(vector<Individual>& pop, crossPointer crossMethod, mutatePointer mutate, const long double pm, const long double px, const int nrPoints, const long double a, const long double b, const long double epsilon, const int dimensions);
void turneyParentSelection(vector<Individual>& pop, crossPointer crossMethod, mutatePointer mutate, const long double pm, const long double px, const int nrPoints, const long double a, const long double b, const long double epsilon, const int dimensions);
void nPointCross(const int nrPoints, vector<Individual>& children, const Individual& parent1, const Individual& parent2, mutatePointer mutate, const long double pm, const long double a, const long double b, const long double epsilon, const int dimensions);
void PrintGenetic(const int dimensions, fitnessPointer fitness, const char* funcName, funcPointer function, selectPointer select, mutatePointer mutate, parentSelectionPointer parentSelection, crossPointer crossMethod, evaluatePointer evaluate, const long double pm, const long double px, const long double pElit, const int nrPoints, const long double a, const long double b, const long double epsilon);

long double fitnessFunctionMin(const point& x, funcPointer function);
long double fitnessFunctionDiv(const point& x, funcPointer function);
bool fitnessBased(const Individual& a, const Individual& b);
bool pxBased(const Individual& a, const Individual& b);