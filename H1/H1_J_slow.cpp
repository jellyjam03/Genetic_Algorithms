#define _USE_MATH_DEFINES
#include <chrono>
#include <iostream>
#include <math.h>
#include <cstring>
#include <iomanip>
#include <random>
#include <time.h>
#include <fstream>
#define MAX_BIT_SIZE 1024
#define MAX_RL_SIZE 256
#define ITERATIONS 100//100 pt schwefel, 10000 pt rastrigin, 1 deJong, 1000 pt michalewicz
#define ITERATIONS_ANN 100

//de scapat de copiere --- sper ca s-a facut
//de calculat f(x) o singura data

using namespace std;

ofstream fout ("h1auxi.txt");

//Other (1/2)
int log2Superior(const long double x);

class point {
public:
    bool bit_form[MAX_BIT_SIZE];
    long double real_form[MAX_RL_SIZE];
    int nrBiti, length, dim;

    point() : bit_form{ 0 }, real_form{ 0 }, nrBiti(0), length(0), dim(0) {}

    point(const long double a, const long double b, const long double epsilon, const int& dimensions) : point() {
        nrBiti = log2Superior((b - a) / epsilon);
        length = nrBiti * dimensions;
        dim = dimensions;
    }

    point(const point& x) {
        nrBiti = x.nrBiti;
        length = x.length;
        dim = x.dim;

        memcpy(this->bit_form, x.bit_form, length);
        memcpy(this->real_form, x.real_form, dim * sizeof(long double));
    }

    void copy(const point& x) {
        memcpy(bit_form, x.bit_form, x.length);
        memcpy(real_form, x.real_form, x.dim * sizeof(long double));

        length = x.length;
        nrBiti = x.nrBiti;
        dim = x.dim;
    }

    void UpdateReal(const long double a, const long double b) {
        unsigned long long int xDecimal;

        for (int i = 0; i < length; i += nrBiti) {
            xDecimal = 0;
            for (int j = i; j - i < nrBiti; j++) {
                xDecimal *= 2;
                xDecimal += bit_form[j];
            }
            real_form[i / nrBiti] = a + xDecimal * (b - a) / ((1 << nrBiti) - 1);
        }
    }

    void BitFlip(const long double a, const long double b, const int poz) {
        unsigned long long int xDecimal = 0;
        int i = 0, offset = poz / nrBiti * nrBiti;
        bit_form[poz] = !bit_form[poz];

        for (i = 0; i < nrBiti; i++) {
            xDecimal *= 2;
            xDecimal += bit_form[offset + i];
        }
        real_form[poz / nrBiti] = a + xDecimal * (b - a) / ((1 << nrBiti) - 1);
    }
};

//functions
long double Dejong(const point& x);
long double Schwefel(const point& x);
long double Rastrigin(const point& x);
long double Michalewicz(const point& x);

//Neighbour selection
long double BestImprovement(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x));
long double WorstImprovement(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x));
long double FirstImprovement(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x));

//RNG
std::random_device rd;
std::seed_seq sq = { rd(), rd(), rd(), rd() };
std::mt19937 mt(sq);

uniform_int_distribution<> coin{ 0, 1 };
uniform_real_distribution<long double> distribution(0.0, 1.0);
void randomCandidate(point& vc, const long double a, const long double b, const long double epsilon, const int& dimensions);
void SelectRandomNeighbour(point& x, long double& fVal, long double (*function)(const point& x), const long double T, const long double a, const long double b);

//Hill Climbing
long double hillClimbing(const int& dimensions, long double (*function)(const point& x), const long double a, const long double b, const long double epsilon, long double (*Improve)(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x)));

//Simulated Annealing
long double simulatedAnnealing(const int& dimensions, long double (*function)(const point& x), const long double a, const long double b, const long double epsilon);
long double UpdateTemperature(const long double temperature, const int& iterr);

//Other (2/2)
void CustomPrint(point& x, long double fx);//for debugging
long double modul(const long double x);
void Print(const int sampleSize, const int dimensions, long double(*method)(const int& dimensions, long double (*function)(const point& x), const long double a, const long double b, const long double epsilon, long double (*Improve)(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x))), long double (*Improve)(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x)), const char* impName, long double (*function)(const point& x), const char* fName, const long double a, const long double b, const long double epsilon);
void PrintAnneal(const int sampleSize, const int dimensions, long double (*function)(const point& x), const char* functionName, const long double a, const long double b, const long double epsilon);

int main() {
    //Print(30, 30, hillClimbing, FirstImprovement, "First Improvement", Michalewicz, "Michalewicz", 0, M_PI, 0.00001);
    int dim[] = {5, 10 , 30};
    for (int i = 0; i < 3; i++) {
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Dejong, "Dejong", -5.12, 5.12, 0.00001);
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Rastrigin, "Rastrigin", -5.12, 5.12, 0.00001);
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Schwefel, "Schwefel", -500, 500, 0.00001);
        cout << dim[i] << '\n';
        PrintAnneal(30, dim[i], Michalewicz, "Michalewicz", 0, M_PI, 0.00001);
    }
    fout.close();
    //cout << setprecision(5) << fixed;
    //cout << hillClimbing(30, Dejong, -5.12, 5.12, 0.00001, BestImprovement) << '\n';
    //cout<<hillClimbing(30, Rastrigin, -5.12, 5.12, 0.00001, BestImprovement)<<'\n';
    //cout<<hillClimbing(30, Michalewicz, 0, M_PI, 0.00001, BestImprovement)<<'\n';
    //cout<<simulatedAnnealing(30, Dejong, -5.12, 5.12, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Schwefel, -500, 500, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Rastrigin, -5.12, 5.12, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Michalewicz, 0, M_PI, 0.00001)<<'\n';

    return 0;
}

long double hillClimbing(const int& dimensions, long double (*function)(const point& x), const long double a, const long double b, const long double epsilon, long double (*Improve)(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x))) {
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

long double simulatedAnnealing(const int& dimensions, long double (*function)(const point& x), const long double a, const long double b, const long double epsilon) {
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

int log2Superior(const long double x) {
    int rez = 0;
    while (x > 1 << rez) rez++;
    return rez;
}

long double modul(const long double x) {
    return x < 0 ? -x : x;
}

long double Dejong(const point& x) {
    int i;
    long double sum = 0;
    for (i = 0; i < x.dim; i++) {
        sum = sum + x.real_form[i] * x.real_form[i];
    }

    return sum;
}

void randomCandidate(point& vc, const long double a, const long double b, const long double epsilon, const int& dimensions) {
    for (int i = 0; i < vc.length; i++) {
        vc.bit_form[i] = coin(mt);
    }

    vc.UpdateReal(a, b);
}

void SelectRandomNeighbour(point& x, long double &fVal, long double (*function)(const point& x), const long double T, const long double a, const long double b) {
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

long double BestImprovement(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x)) {
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

long double WorstImprovement(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x)) {
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

long double FirstImprovement(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x)) {
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

void CustomPrint(point& x, long double fx) {
    cout << "Coordinates: ";
    for (int i = 0; i < x.dim; i++) cout << x.real_form[i] << ' ';
    cout << "\nBit string: ";
    for (int i = 0; i < x.length; i++) cout << x.bit_form[i];
    cout << "\nValue: " << fx << '\n';

}

long double Schwefel(const point& x) {
    long double sum = 0;
    for (int i = 0; i < x.dim; i++) {
        sum = sum - x.real_form[i] * sin(sqrt(abs(x.real_form[i])));
    }

    return sum;
}

long double Rastrigin(const point& x) {
    long double sum = 10 * x.length / x.nrBiti;
    for (int i = 0; i < x.dim; i++) {
        sum = sum + x.real_form[i] * x.real_form[i] - 10 * cos(2 * M_PI * x.real_form[i]);
    }

    return sum;
}

long double Michalewicz(const point& x) {
    long double sum = 0;

    for (int i = 0; i < x.dim; i++)
        sum = sum + sin(x.real_form[i]) * pow(sin(i * pow(x.real_form[i], 2) / M_PI), 20);

    return -sum;
}
// std::chrono::steady_clock
void Print(const int sampleSize, const int dimensions, long double(*method)(const int& dimensions, long double (*function)(const point& x), const long double a, const long double b, const long double epsilon, long double (*Improve)(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x))), long double (*Improve)(point& x, const long double fVal, const long double a, const long double b, long double (*function)(const point& x)), const char* impName, long double (*function)(const point& x), const char* fName, const long double a, const long double b, const long double epsilon) {
    chrono::steady_clock timer;
    chrono::steady_clock::time_point begin, end;
    int i;
    fout << setprecision(5) << fixed;
    for (i = 0; i < sampleSize; i++) {
        begin = timer.now();
        fout << fName << "'s function:" << dimensions << ':' << method(dimensions, function, a, b, epsilon, Improve) << ':';
        end = timer.now();
        fout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << ':' << impName << '\n';
        cout << i + 1 << '\n';
    }
    fout.close();
}

void PrintAnneal(const int sampleSize, const int dimensions, long double (*function)(const point& x), const char* functionName, const long double a, const long double b, const long double epsilon) {
    chrono::steady_clock timer;
    chrono::steady_clock::time_point begin, end;
    int i;
    fout << setprecision(5) << fixed;
    for (i = 0; i < sampleSize; i++) {
        begin = timer.now();
        fout << functionName << "'s function:" << dimensions << ':' << simulatedAnnealing(dimensions, function, a, b, epsilon) << ':';
        end = timer.now();
        fout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << ':' << "Simulated Annealing" << '\n';
        //cout << i + 1 << '\n';
    }
}
