#include "HillClimbing.h"

using namespace std;

ofstream fout1("HillClimb.txt");

long double BestImprovement(point& x, const long double fVal, const long double a, const long double b, funcPointer function) {
    long double fReturn = fVal, fPos;
    bool isInside;
    int bestBit = -1;

    for (int i = 0; i < x.length; i++) {
        x.BitFlip(a, b, i);

        isInside = true;
        for (int i = 0; i < x.dim; i++)
            if (x.real_form[i] < a || b < x.real_form[i])
                isInside = false;
        if (!isInside)
            continue;
        fPos = function(x);
        if (fPos < fReturn) {
            bestBit = i;
            fReturn = fPos;
        }
        x.BitFlip(a, b, i);
    }

    if (bestBit != -1) {
        x.BitFlip(a, b, bestBit);
    }

    return fReturn;
}

long double WorstImprovement(point& x, const long double fVal, const long double a, const long double b, funcPointer function) {
    long double fReturn = fVal, fPos;
    bool isInside, improved = false;
    int worstBit = -1;

    for (int i = 0; i < x.length; i++) {
        x.BitFlip(a, b, i);

        isInside = true;
        for (int i = 0; i < x.dim; i++)
            if (x.real_form[i] < a || b < x.real_form[i])
                isInside = false;
        if (!isInside)
            continue;

        fPos = function(x);
        if (fPos < fVal)
            if (!improved) {
                fReturn = fPos;
                worstBit = i;
                improved = 1;
            }
            else
                if (fPos > fReturn) {
                    fReturn = fPos;
                    worstBit = i;
                }
        x.BitFlip(a, b, i);
    }

    if (worstBit != -1) {
        x.BitFlip(a, b, worstBit);
    }

    return fReturn;
}

long double FirstImprovement(point& x, const long double fVal, const long double a, const long double b, funcPointer function) {
    long double fPos;
    bool isInside;

    for (int i = 0; i < x.length; i++) {
        x.BitFlip(a, b, i);

        isInside = true;
        for (int i = 0; i < x.dim; i++)
            if (x.real_form[i] < a || b < x.real_form[i])
                isInside = false;
        if (!isInside)
            continue;

        fPos = function(x);
        if (fPos < fVal) {
            return fPos;
        }
        x.BitFlip(a, b, i);
    }
    return fVal;
}

long double hillClimbing(const int& dimensions, funcPointer function, const long double a, const long double b, const long double epsilon, improvePointer Improve) {
    int t = 0;
    point best(a, b, epsilon, dimensions), vc(a, b, epsilon, dimensions), vn(a, b, epsilon, dimensions);
    bool local;
    long double fVal = 1000000, fNeighVal = 1000000, fBest;

    randomCandidate(best, a, b, epsilon, dimensions);
    fBest = function(best);
    do {
        local = false;
        randomCandidate(vc, a, b, epsilon, dimensions);
        fVal = function(vc);
        do {
            fNeighVal = Improve(vc, fVal, a, b, function);
            if (fNeighVal < fVal) {
                fVal = fNeighVal;
            }
            else
                local = true;
        } while (!local);
        t++;
        if (fVal < fBest)
            fBest = fVal;
    } while (t < ITERATIONS);

    return fBest;
}

void Print(const int sampleSize, const int dimensions, improvePointer Improve, const char* impName, funcPointer function, const char* fName, const long double a, const long double b, const long double epsilon) {
    chrono::steady_clock timer;
    chrono::steady_clock::time_point begin, end;
    int i;
    fout1 << setprecision(5) << fixed;
    for (i = 0; i < sampleSize; i++) {
        begin = timer.now();
        fout1 << fName << "'s function:" << dimensions << ':' << hillClimbing(dimensions, function, a, b, epsilon, Improve) << ':';
        end = timer.now();
        fout1 << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << ':' << impName << '\n';
        system("cls");
        cout << i + 1 / sampleSize * 100 << "% Completed" << '\n';
    }
    fout1.close();
}