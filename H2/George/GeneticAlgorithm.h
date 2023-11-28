#pragma once
#include "Common.h"
#include "Individual.h"
#define POP_SIZE 30
#define GENETIC_ITERATIONS 1000


typedef long double (*funcPointer)(const point& x);
typedef long double (*fitnessPointer)(const point& x, funcPointer function);
typedef void (*selectPointer)(vector<Individual>& pop, const long double pElit);
typedef void (*mutatePointer)(vector<Individual>& pop, const long double pm, const long double a, const long double b);
typedef void (*crossPointer)(Individual& child1, Individual& child2, const Individual& parent1, const Individual& parent2, const long double a, const long double b);
typedef void (*parentSelectionPointer)(vector<Individual>& pop, crossPointer crossMethod, const long double px, const long double a, const long double b, const long double epsilon, const int dimensions);
typedef void (*evaluatePointer)(vector<Individual>& pop, fitnessPointer fitness, funcPointer function);


long double geneticAlgorithm(const int dimensions, fitnessPointer fitness, funcPointer function, selectPointer select, mutatePointer mutate, parentSelectionPointer parentSelection, crossPointer crossMethod, evaluatePointer evaluate, const long double pm, const long double px, const long double pElit, const long double a, const long double b, const long double epsilon);
void evaluate(vector<Individual>& pop, fitnessPointer fitness, funcPointer function);
void unifromMutate(vector<Individual>& pop, const long double pm, const long double a, const long double b);
void selectWheelOfFortune(vector<Individual>& pop, const long double pElit);
void sortParentSelection(vector<Individual>& pop, crossPointer crossMethod, const long double px, const long double a, const long double b, const long double epsilon, const int dimensions);
void uniformCross(Individual& child1, Individual& child2, const Individual& parent1, const Individual& parent2, const long double a, const long double b);
void onePointCross(Individual& child1, Individual& child2, const Individual& parent1, const Individual& parent2, const long double a, const long double b);
void twoPointCross(Individual& child1, Individual& child2, const Individual& parent1, const Individual& parent2, const long double a, const long double b);

long double fitnessFunction(const point& x, funcPointer function);
bool fitnessBased(const Individual& a, const Individual& b);
bool pxBased(const Individual& a, const Individual& b);