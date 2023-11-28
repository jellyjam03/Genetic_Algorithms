#include "GeneticAlgorithm.h"

long double fitnessFunction(const point& x, funcPointer function) {
    long double aux = function(x);
    //cout << "Valoarea la care se imparte in fitnessFunction: " << aux << '\n';
    return -aux;
}

//for sorting
bool fitnessBased(const Individual& a, const Individual& b) { //sorting condition for the wheel of fortune
    return a.fitness > b.fitness;
}

bool pxBased(const Individual& a, const Individual& b) {
    return a.px < b.px;
}

long double geneticAlgorithm(const int dimensions, fitnessPointer fitness, funcPointer function, selectPointer select, mutatePointer mutate, parentSelectionPointer parentSelection, crossPointer crossMethod, evaluatePointer evaluate, const long double pm, const long double px, const long double pElit, const long double a, const long double b, const long double epsilon) {
    int t = 0, i = 0;
    Individual p(a, b, epsilon, dimensions);
    vector<Individual> pop;
    long double totalFitness = 0, bestFitness = 0;

    //generate the starting population & calculate the fitness of it
    for (i = 0; i < POP_SIZE; i++) {
        randomCandidate(p.x, a, b, epsilon, dimensions);
        pop.push_back(p);
    }
    evaluate(pop, fitness, function);

    while (t < GENETIC_ITERATIONS) {
        t = t + 1;
        select(pop, pElit);
        mutate(pop, pm, a, b);
        parentSelection(pop, crossMethod, px, a, b, epsilon, dimensions);
        evaluate(pop, fitness, function);
    }

    int bestIndex = 0;

    for (i = 1; i < pop.size(); i++)
        if (pop[i].fitness > pop[bestIndex].fitness)
            bestIndex = i;
    return function(pop[bestIndex].x);
}

void evaluate(vector<Individual>& pop, fitnessPointer fitness, funcPointer function) {
    int i = 0;
    for (i = 0; i < pop.size(); i++) {
        pop[i].fitness = fitness(pop[i].x, function);
        //cout << "Valoarea care ajunge in pop[i].fitness: " << pop[i].fitness << '\n';
    }
}

void unifromMutate(vector<Individual>& pop, const long double pm, const long double a, const long double b) {
    int i = 0, j = 0;
    for (i = 0; i < pop.size(); i++) {
        for (j = 0; j < pop[i].x.length; j++)
            if (distribution(mt) < pm) {
                pop[i].x.BitFlip(a, b, j);
            }
    }
}

void selectWheelOfFortune(vector<Individual>& pop, const long double pElit) {
    int i, j, nrElit = 0;
    long double p, totalFitness = 0, sumFitness = 0;
    bool found;
    vector<Individual> newPop;

    for (i = 0; i < pop.size(); i++) {
        totalFitness += pop[i].fitness;
    }
    sort(pop.begin(), pop.end(), fitnessBased);

    //we apply elitism and select the first pElit*pop.size() individuals to live in the next generation
    nrElit = min((int)(pElit * pop.size()), (int)POP_SIZE);
    for (i = 0; i < nrElit; i++)
        newPop.push_back(pop[i]);

    for (i = 0, sumFitness = 0; i < POP_SIZE - nrElit; i++) {//we select POP_SIZE individuals for the next generation
        p = distribution(mt);
        for (j = 0, sumFitness = 0, found = false; j < pop.size() && !found; j++) {//we search to see who won the wheel
            sumFitness += pop[i].fitness;
            if (sumFitness / totalFitness > p) {
                newPop.push_back(pop[j]);
                found = 1;
            }
        }
        if (!found) {
            newPop.push_back(pop[pop.size() - 1]);
        }
    }

    pop.clear();
    for (i = 0; i < newPop.size(); i++) {
        pop.push_back(newPop[i]);
    }
}

void sortParentSelection(vector<Individual>& pop, crossPointer crossMethod, const long double px, const long double a, const long double b, const long double epsilon, const int dimensions) {
    Individual child1(a, b, epsilon, dimensions), child2(a, b, epsilon, dimensions);
    int i, initialPopSize = pop.size();

    for (i = 0; i < initialPopSize; i++)
        pop[i].px = distribution(mt);
    sort(pop.begin(), pop.end(), pxBased);

    for (i = 0; i < initialPopSize; i += 2)
        if (pop[i].px < px && i + 1 < initialPopSize)
            if (pop[i + 1].px < px || coin(mt) == 0) {
                crossMethod(child1, child2, pop[i], pop[i + 1], a, b);
                pop.push_back(child1);
                pop.push_back(child2);
            }
            else
                break;
}

void uniformCross(Individual& child1, Individual& child2, const Individual& parent1, const Individual& parent2, const long double a, const long double b) {
    int i = 0;

    for (i = 0; i < child1.x.length; i++)
        if (distribution(mt) < 0.5) {
            child1.x.bit_form[i] = parent1.x.bit_form[i];
            child2.x.bit_form[i] = parent2.x.bit_form[i];
        }
        else {
            child1.x.bit_form[i] = parent2.x.bit_form[i];
            child2.x.bit_form[i] = parent1.x.bit_form[i];
        }

    child1.x.UpdateReal(a, b);
    child2.x.UpdateReal(a, b);
}

void onePointCross(Individual& child1, Individual& child2, const Individual& parent1, const Individual& parent2, const long double a, const long double b) {
    uniform_int_distribution<int> indeces(0, parent1.x.length - 1);
    int index = indeces(mt), i;

    for (i = 0; i < child1.x.length; i++)
        if (i <= index) {
            child1.x.bit_form[i] = parent1.x.bit_form[i];
            child2.x.bit_form[i] = parent2.x.bit_form[i];
        }
        else {
            child1.x.bit_form[i] = parent2.x.bit_form[i];
            child2.x.bit_form[i] = parent1.x.bit_form[i];
        }
    child1.x.UpdateReal(a, b);
    child2.x.UpdateReal(a, b);
}

void twoPointCross(Individual& child1, Individual& child2, const Individual& parent1, const Individual& parent2, const long double a, const long double b) {
    uniform_int_distribution<int> indeces(0, parent1.x.length - 1);
    int index1 = indeces(mt), index2 = indeces(mt), i;
    if (index1 > index2) swap(index1, index2);

    for (i = 0; i < child1.x.length; i++)
        if (i < index1 || index2 < i) {
            child1.x.bit_form[i] = parent1.x.bit_form[i];
            child2.x.bit_form[i] = parent2.x.bit_form[i];
        }
        else {
            child1.x.bit_form[i] = parent2.x.bit_form[i];
            child2.x.bit_form[i] = parent1.x.bit_form[i];
        }
    child1.x.UpdateReal(a, b);
    child2.x.UpdateReal(a, b);
}