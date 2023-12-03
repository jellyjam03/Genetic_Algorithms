#include "GeneticAlgorithm.h"

ofstream fout3("GenetimAlg.txt");

long double fitnessFunctionDiv(const point& x, funcPointer function) {
    return 1 / function(x);
}

long double fitnessFunctionMin(const point& x, funcPointer function) {
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

long double geneticAlgorithm(const int dimensions, fitnessPointer fitness, funcPointer function, selectPointer select, mutatePointer mutate, parentSelectionPointer parentSelection, crossPointer crossMethod, evaluatePointer evaluate, const long double pm, const long double px, const long double pElit, const int nrPoints, const long double a, const long double b, const long double epsilon) {
    int t = 0, i = 0, reignLen = 0;
    Individual p(a, b, epsilon, dimensions), best(a, b, epsilon, dimensions);
    long double pm1 = pm, pm2 = pm + 1.0 / p.x.length, pmf = pm;
    vector<Individual> pop;
    long double totalFitness = 0, bestFitness = 0;

    //generate the starting population & calculate the fitness of it
    for (i = 0; i < POP_SIZE; i++) {
        randomCandidate(p.x, a, b, epsilon, dimensions);
        pop.push_back(p);
    }
    evaluate(pop, fitness, function);
    best = pop[0];

    while (t < GENETIC_ITERATIONS) {
        select(pop, pElit);
        //cout << fitness(pop[0].x, function) << ' ';
        /*if (t % 50 == 0) {
            cout << '\n';
            for (i = 0; i < pop.size(); i++)
                cout << Michalewicz(pop[i].x) << '\n';
            cout << '\n';
        }*/
        if (best.fitness == pop[0].fitness) {
            if (reignLen >= 100) {
                pmf = pm2;
                //cout << "MARIT\n";
            }
            reignLen++;
        }
        else {
            best = pop[0];
            reignLen = 0;
            pmf = pm1;
        }
        mutate(pop, pmf, a, b);
        parentSelection(pop, crossMethod, mutate, pmf, px, nrPoints, a, b, epsilon, dimensions);
        evaluate(pop, fitness, function);
        t = t + 1;
    }

    int bestIndex = 0;

    for (i = 1; i < pop.size(); i++)
        if (pop[i].fitness > pop[bestIndex].fitness)
            bestIndex = i;
    return function(pop[bestIndex].x);
}

//long double geneticAlgorithm(const int dimensions, fitnessPointer fitness, funcPointer function, selectPointer select, mutatePointer mutate, parentSelectionPointer parentSelection, crossPointer crossMethod, evaluatePointer evaluate, const long double pm, const long double px, const long double pElit, const long double a, const long double b, const long double epsilon) {
//    int t = 0, i = 0, reignLen = 0;
//    Individual p(a, b, epsilon, dimensions), best(a, b, epsilon, dimensions);
//    long double pm1 = pm, pm2 = pm + 1.0 / p.x.length, pmf = pm;
//    vector<Individual> pop;
//    long double totalFitness = 0, bestFitness = 0;
//
//    //generate the starting population & calculate the fitness of it
//    for (i = 0; i < POP_SIZE; i++) {
//        randomCandidate(p.x, a, b, epsilon, dimensions);
//        pop.push_back(p);
//    }
//    evaluate(pop, fitness, function);
//    best = pop[0];
//
//    while (reignLen < 100) {
//        select(pop, pElit);
//        /*if (t % 50 == 0) {
//            cout << '\n';
//            for (i = 0; i < pop.size(); i++)
//                cout << Michalewicz(pop[i].x) << '\n';
//            cout << '\n';
//        }*/
//        if (best.fitness == pop[0].fitness) {
//            if (reignLen >= 50)
//                pmf = pm2;
//            reignLen++;
//        }
//        else {
//            best = pop[0];
//            reignLen = 0;
//            pmf = pm1;
//        }
//        mutate(pop, pmf, a, b);
//        parentSelection(pop, crossMethod, mutate, pmf, px, a, b, epsilon, dimensions);
//        evaluate(pop, fitness, function);
//        t = t + 1;
//    }
//
//    int bestIndex = 0;
//
//    for (i = 1; i < pop.size(); i++)
//        if (pop[i].fitness > pop[bestIndex].fitness)
//            bestIndex = i;
//    cout << t << '\n';
//    return function(pop[bestIndex].x);
//}

void evaluate(vector<Individual>& pop, fitnessPointer fitness, funcPointer function) {
    int i = 0;
    for (i = 0; i < pop.size(); i++) {
        pop[i].fitness = fitness(pop[i].x, function);
        //cout << "Valoarea care ajunge in pop[i].fitness: " << pop[i].fitness << '\n';
    }
}

void unifromMutate(vector<Individual>& pop, const long double pm, const long double a, const long double b) {
    int i = 0, j = 0;
    long double fitnessAvg = 0, pf = pm;
    for (i = 0; i < pop.size(); i++)
        fitnessAvg += pop[i].fitness;
    fitnessAvg /= pop.size();

    //if (fitnessAvg) cout << fitnessAvg << '\n';

    for (i = 1; i < pop.size(); i++) {
        if (pop[i].fitness < fitnessAvg)
            pf = pm + 1.0 / pop[i].x.length;
        else
            pf = pm;
        for (j = 0; j < pop[i].x.length; j++)
            if (distribution(mt) < pf) {
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
    newPop.push_back(pop[pop.size() - 1]); nrElit++;

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
    sort(pop.begin(), pop.end(), fitnessBased);
}

void sortParentSelection(vector<Individual>& pop, crossPointer crossMethod, mutatePointer mutate, const long double pm, const long double px, const int nrPoints, const long double a, const long double b, const long double epsilon, const int dimensions) {
    Individual child1(a, b, epsilon, dimensions), child2(a, b, epsilon, dimensions);
    vector<Individual> children;
    int i, j;

    for (i = 0; i < POP_SIZE; i++)
        pop[i].px = distribution(mt);
    sort(pop.begin(), pop.end(), pxBased);

    for (i = 0; i < POP_SIZE; i += 2)
        if (pop[i].px < px && i + 1 < POP_SIZE)
            if (pop[i + 1].px < px || coin(mt) == 0) {
                crossMethod(nrPoints, children, pop[i], pop[i + 1], mutate, pm, a, b, epsilon, dimensions);
                for (j = 0; j < children.size(); j++)
                    pop.push_back(children[j]);
                children.clear();
            }
            else
                break;
}

void selectGreedy(vector<Individual>& pop, const long double pElit) {
    sort(pop.begin(), pop.end(), fitnessBased);
    while (pop.size() > POP_SIZE)
        pop.pop_back();
}

void turneyParentSelection(vector<Individual>& pop, crossPointer crossMethod, mutatePointer mutate, const long double pm, const long double px, const int nrPoints, const long double a, const long double b, const long double epsilon, const int dimensions) {
    int i, poz, j;
    Individual aux(a, b, epsilon, dimensions);
    vector<Individual> children, newPop;

    for (i = 0; i < pop.size(); i++) {//shuffle
        poz = i + rand()%(pop.size() - i);
        aux = pop[poz];
        pop[poz] = pop[i];
        pop[i] = aux;
    }
    //eventual de modificat sa scot restul de 3 potentiali parinti care nu se incadreaza
    for (i = 0; i < POP_SIZE; i += 5) {
        sort(pop.begin() + i, pop.begin() + i + 5, fitnessBased);
        newPop.push_back(pop[i]);
        newPop.push_back(pop[i + 1]);
        crossMethod(nrPoints, children, pop[i], pop[i + 1], mutate, pm, a, b, epsilon, dimensions);
        for (j = 0; j < children.size(); j++)
            newPop.push_back(children[j]);
        /*cout << "Inside parentSelect:\n";
        for (j = 0; j < children.size(); j++)
            cout << fitnessFunctionMin(children[j].x, Michalewicz) << '\n';
        cout << '\n';*/
        children.clear();
    }

    pop.clear();
    for (i = 0; i < newPop.size(); i++)
        pop.push_back(newPop[i]);
}

void nPointCross(const int nrPoints, vector<Individual>& children, const Individual& parent1, const Individual& parent2, mutatePointer mutate, const long double pm, const long double a, const long double b, const long double epsilon, const int dimensions) {
    //uniform_int_distribution<int> indeces(0, parent1.x.length - 1);
    Individual child1(a, b, epsilon, dimensions), child2(a, b, epsilon, dimensions);
    vector<Individual> aux;
    int i, currentIndex = 0;
    bool flip = coin(mt);

    if (nrPoints < 1 || parent1.x.length < nrPoints)
        return;

    vector<int> indeces(parent1.x.length);
    iota(indeces.begin(), indeces.end(), 0);
    random_shuffle(indeces.begin(), indeces.end());

    vector<int> Points;
    for (i = 0; i < nrPoints; i++)
        Points.push_back(indeces[i]);
    sort(Points.begin(), Points.end());

    for (i = 0; i < child1.x.length; i++) {
        if (i > Points[currentIndex]) {
            currentIndex++;
            flip = coin(mt);
        }
        if (flip == 0 /*currentIndex % 2 == 0*/) { //dam sansa egala
            child1.x.bit_form[i] = parent1.x.bit_form[i];
            child2.x.bit_form[i] = parent2.x.bit_form[i];
        }
        else {
            child1.x.bit_form[i] = parent2.x.bit_form[i];
            child2.x.bit_form[i] = parent1.x.bit_form[i];
        }
    }
    child1.x.UpdateReal(a, b);
    child2.x.UpdateReal(a, b);

    aux.push_back(child1);
    aux.push_back(child2);
    mutate(aux, pm, a, b);

    children.push_back(child1);
    children.push_back(child2);
    children.push_back(aux[0]);
    children.push_back(aux[1]);
}

void fitnessParentSelection(vector<Individual>& pop, crossPointer crossMethod, mutatePointer mutate, const long double pm, const long double px, const int nrPoints, const long double a, const long double b, const long double epsilon, const int dimensions) {
    Individual child1(a, b, epsilon, dimensions), child2(a, b, epsilon, dimensions);
    vector<Individual> children;
    int i, j, nrParents = pop.size() * px;

    sort(pop.begin(), pop.end(), fitnessBased);

    vector<int> indeces(nrParents);
    iota(indeces.begin(), indeces.end(), 0);

    nrParents = nrParents + coin(mt);
    if (nrParents < 2) return;

    for (i = 1; i <= nrParents; i++) {
        random_shuffle(indeces.begin(), indeces.end());

        crossMethod(nrPoints, children, pop[indeces[0]], pop[indeces[1]], mutate, pm, a, b, epsilon, dimensions);
        for (j = 0; j < children.size(); j++)
            pop.push_back(children[j]);
        children.clear();
    }
}

void PrintGenetic(const int dimensions, fitnessPointer fitness, const char* funcName, funcPointer function, selectPointer select, mutatePointer mutate, parentSelectionPointer parentSelection, crossPointer crossMethod, evaluatePointer evaluate, const long double pm, const long double px, const long double pElit, const int nrPoints, const long double a, const long double b, const long double epsilon) {
    chrono::steady_clock timer;
    chrono::steady_clock::time_point begin, end;
    int i;
    fout3 << setprecision(5) << fixed;

    for (i = 0; i < 30; i++) {
        begin = timer.now();
        fout3 << funcName << ':' << dimensions << ':' << geneticAlgorithm(dimensions, fitness, function, select, mutate, parentSelection, crossMethod, evaluate, pm, px, pElit, nrPoints, a, b, epsilon) << ':';
        end = timer.now();
        fout3 << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << ":Genetic Algorithm\n";
        cout << i+1 << ' ';
    }
    cout << '\n';
}