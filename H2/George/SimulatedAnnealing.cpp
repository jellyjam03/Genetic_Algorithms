#include "SimulatedAnnealing.h"

ofstream fout2("SimAnneal.txt");

long double simulatedAnnealing(const int& dimensions, funcPointer function, const long double a, const long double b, const long double epsilon) {
    int t = 0, iterations = 0;
    long double T = 10000.0;
    point vc(a, b, epsilon, dimensions);
    long double fVal, fBest;

    randomCandidate(vc, a, b, epsilon, dimensions);
    fVal = fBest = function(vc);
    do {
        do {
            SelectRandomNeighbour(vc, fVal, function, T, a, b);
            if (fVal < fBest)
                fBest = fVal;
            iterations++;
        } while (iterations < t * ITERATIONS_ANN);//faptul ca iteratiile de aici nu schimba cu mult outcome-ul
        t = t + 1;
        T = UpdateTemperature(T, t);
    } while (T > epsilon);
    return fBest;
}

long double UpdateTemperature(const long double temperature, const int& iterr) {
    //return temperature * (1 - 0.0001 * iterr);
    return temperature * 0.999;//de mentionat diferenta la temperatura initiala + cooling relativ la iteratia curenta
}

void SelectRandomNeighbour(point& x, long double& fVal, funcPointer function, const long double T, const long double a, const long double b) {
    uniform_int_distribution<> neighbours{ 0, x.length - 1 };

    int randPoz = neighbours(mt);
    long double fNeigh;
    x.BitFlip(a, b, randPoz);
    fNeigh = function(x);

    if (fNeigh < fVal)
        fVal = fNeigh;
    else
        if (distribution(mt) < exp(-modul(fNeigh - fVal) / T))
            fVal = fNeigh;
        else
            x.BitFlip(a, b, randPoz);
}

long double modul(const long double x) {
    return x < 0 ? -x : x;
}

void PrintAnneal(const int sampleSize, const int dimensions, funcPointer function, const char* functionName, const long double a, const long double b, const long double epsilon) {
    chrono::steady_clock timer;
    chrono::steady_clock::time_point begin, end;
    int i;
    fout2 << setprecision(5) << fixed;
    for (i = 0; i < sampleSize; i++) {
        begin = timer.now();
        fout2 << functionName << "'s function:" << dimensions << ':' << simulatedAnnealing(dimensions, function, a, b, epsilon) << ':';
        end = timer.now();
        fout2 << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << ':' << "Simulated Annealing" << '\n';
        //cout << i + 1 << '\n';
    }
}