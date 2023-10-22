#include <iostream>
#include <math.h>
#include <cstring>
#include <iomanip>
#include <random>
#include <time.h>
#define MAX_BIT_SIZE 1024
#define MAX_RL_SIZE 256
#define ITERATIONS 100
#define ITERATIONS_ANN 10000

using namespace std;

//Other (1/2)
int log2Superior(const long double& x);

class point {
    public:
    bool bit_form[MAX_BIT_SIZE];
    long double real_form[MAX_RL_SIZE];
    int nrBiti, length, dim;

    point() : bit_form{0}, real_form{0}, nrBiti(0), length(0), dim(0) {}

    point(const long double& a, const long double& b, const long double& epsilon, const int& dimensions) {
        nrBiti = log2Superior((b-a)/epsilon);
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

    void UpdateReal(const long double& a, const long double& b) {
        unsigned long long int xDecimal;

        for (int i = 0; i < length; i += nrBiti) {
            xDecimal = 0;
            for (int j = i; j - i < nrBiti; j++) {
                xDecimal *= 2;
                xDecimal += bit_form[j];
            }
            real_form[i/nrBiti] = a + xDecimal * (b - a)/((1<<nrBiti) - 1);
        }
    }
};

//functions
long double Dejong(const point& x);
long double Schwefel(const point& x);
long double Rastrigin(const point& x);
long double Michalewicz(const point& x);

//Neighbour selection
void BestImprovement(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x));
void WorstImprovement(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x));
void FirstImprovement(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x));

//RNG
mt19937 mt(random_device{}());
uniform_int_distribution coin{0, 1};
uniform_real_distribution<long double> distribution(0.0, 1.0);
void randomCandidate(point& vc, const long double& a, const long double& b, const long double epsilon, const int& dimensions);
void SelectRandomNeighbour(const point& x, point& destination, const long double& a, const long double& b);

//Hill Climbing
long double hillClimbing(const int& dimensions, long double (*function)(const point& x), const long double& a, const long double& b, const long double& epsilon,  void (*Improve)(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x)));

//Simulated Annealing
long double simulatedAnnealing(const int& dimensions, long double (*function)(const point& x), const long double& a, const long double& b, const long double& epsilon);
long double UpdateTemperature(const long double& temperature, const int& iterr);

//Other (2/2)
void CustomPrint(point& x, long double fx);
long double modul(const long double& x);

int main() {
    cout << setprecision(7) << fixed;
    //cout<<hillClimbing(30, Dejong, -5.12, 5.12, 0.00001, WorstImprovement)<<'\n';
    cout<<hillClimbing(30, Schwefel, -500, 500, 0.00001, BestImprovement)<<'\n';
    //cout<<hillClimbing(30, Rastrigin, -5.12, 5.12, 0.00001, FirstImprovement)<<'\n';
    //cout<<hillClimbing(30, Michalewicz, 0, M_PI, 0.00001, BestImprovement)<<'\n';
    //cout<<simulatedAnnealing(30, Dejong, -5.12, 5.12, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Schwefel, -500, 500, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Rastrigin, -5.12, 5.12, 0.00001)<<'\n';
    //cout<<simulatedAnnealing(30, Michalewicz, 0, M_PI, 0.00001)<<'\n';
    return 0;
}

long double hillClimbing(const int& dimensions, long double (*function)(const point& x), const long double& a, const long double& b, const long double& epsilon,  void (*Improve)(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x))) {
    int t = 0;
    point best(a, b, epsilon, dimensions), vc(a, b, epsilon, dimensions), vn(a, b, epsilon, dimensions);
    bool local;

    randomCandidate(best, a, b, epsilon, dimensions);
    do {
        local = false;
        randomCandidate(vc, a, b, epsilon, dimensions);
        do {
            Improve(vc, vn, a, b, function);
            if (function(vn) < function(vc)) {
                vc.copy(vn);
            }
            else
                local = true;
        }while(!local);
        t++;
        if (function(vc) < function(best))
            best.copy(vc);
    }while(t < ITERATIONS);

    return function(best);
}

long double simulatedAnnealing(const int& dimensions, long double (*function)(const point& x), const long double& a, const long double& b, const long double& epsilon) {
    int t = 0, iterations = 0;
    long double T = 1.0;
    point vc(a, b, epsilon, dimensions), vn(a, b, epsilon, dimensions), best(a, b, epsilon, dimensions);

    randomCandidate(vc, a, b, epsilon, dimensions);
    best.copy(vc);
    do {
        do {
            if (function(vc) < function(best)) 
                best.copy(vc);

            SelectRandomNeighbour(vc, vn, a, b);
            if (function(vc) < function(vn))
                vn.copy(vc);
                else
                if (distribution(mt) < exp(-modul(function(vn) - function(vc)) / T))
                    vn.copy(vc);
            iterations++;
        } while (iterations < t*ITERATIONS_ANN);
        t = t + 1;
        T = UpdateTemperature(T, t);
    } while (T > epsilon);
    return function(best);
}

long double UpdateTemperature(const long double& temperature, const int& iterr) {
    return temperature * (1 - 0.01 * iterr);
}

int log2Superior(const long double& x) {
    int rez = 0;
    while (x > 1<<rez) rez++;
    return rez;
}

long double modul(const long double& x) {
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

void randomCandidate(point& vc, const long double& a, const long double& b, const long double epsilon, const int& dimensions) {
    for (int i = 0; i < vc.length; i++) {
        vc.bit_form[i] = coin(mt);
    }

    vc.UpdateReal(a, b);
}

void SelectRandomNeighbour(const point& x, point& destination, const long double& a, const long double& b) {
    uniform_int_distribution neighbours {0, x.length - 1};

    int randPoz = neighbours(mt);
    destination.copy(x);
    destination.bit_form[randPoz] = !destination.bit_form[randPoz];
    destination.UpdateReal(a, b);
}

void BestImprovement(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x)) {
    destination.copy(x);
    point vn(x);
    bool isInside;

    for (int i = 0; i < x.length; i++) {
        if (i > 0) vn.bit_form[i-1] = !vn.bit_form[i-1];
        vn.bit_form[i] = !x.bit_form[i];
        vn.UpdateReal(a, b);

        isInside = true;
        for (int i = 0; i < x.dim; i++)
            if (x.real_form[i] < a || b < x.real_form[i])
                isInside = false;
        if (!isInside)
            continue;

        if (function(vn) < function(destination)) {
            destination.copy(vn);
        }
    }
}

void WorstImprovement(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x)) {
    destination.copy(x);
    point vn(x);
    bool isInside, improved = false;

    for (int i = 0; i < x.length; i++) {
        if (i > 0) vn.bit_form[i-1] = !vn.bit_form[i-1];
        vn.bit_form[i] = !x.bit_form[i];
        vn.UpdateReal(a, b);

        isInside = true;
        for (int i = 0; i < x.dim; i++)
            if (x.real_form[i] < a || b < x.real_form[i])
                isInside = false;
        if (!isInside)
            continue;

        if (function(vn) < function(x))
            if (!improved) {
                destination.copy(vn);
                improved = 1;
            }
            else
            if (function(vn) > function(destination)) {
                destination.copy(vn);
            }
    }
}

void FirstImprovement(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x)) {
    destination.copy(x);
    point vn(x);
    bool isInside;

    for (int i = 0; i < x.length; i++) {
        if (i > 0) vn.bit_form[i-1] = !vn.bit_form[i-1];
        vn.bit_form[i] = !x.bit_form[i];
        vn.UpdateReal(a, b);

        isInside = true;
        for (int i = 0; i < x.dim; i++)
            if (x.real_form[i] < a || b < x.real_form[i])
                isInside = false;
        if (!isInside)
            continue;

        if (function(vn) < function(destination)) {
            destination.copy(vn);
            return;
        }
    }
}

void CustomPrint(point& x, long double fx) {
    cout<<"Coordinates: ";
    for (int i = 0; i < x.dim; i++) cout<<x.real_form[i] << ' ';
    cout<<"\nBit string: ";
    for (int i = 0; i < x.length; i++) cout<<x.bit_form[i];
    cout<<"\nValue: "<<fx<<'\n';

}

long double Schwefel(const point& x) {
    long double sum = 0;
    for (int i = 0; i < x.dim; i++) {
        sum = sum - x.real_form[i]*sin(sqrt(abs(x.real_form[i])));
    }

    return sum;
}

long double Rastrigin(const point& x) {
    long double sum = 10 * x.length/x.nrBiti;
    for (int i = 0; i < x.dim; i++) {
        sum = sum + x.real_form[i] * x.real_form[i] - 10 * cos(2*M_PI * x.real_form[i]);
    }

    return sum;
}

long double Michalewicz(const point& x) {
    long double sum = 0;
    
    for (int i = 0; i < x.dim; i++) 
        sum = sum + sin(x.real_form[i]) * pow(sin(i * pow(x.real_form[i], 2) / M_PI), 20);

    return -sum;
}
