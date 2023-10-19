#include <iostream>
#include <math.h>
#include <cstring>
#include <iomanip>
#include <random>
#include <time.h>
#define MAX 100

using namespace std;

//Other (1/2)
int log2Superior(const long double& x);

class point {
    public:
    bool* bit_form;
    long double* real_form;
    int nrBiti, length, dim;

    point() : bit_form(NULL), real_form(NULL), nrBiti(0), length(0), dim(0) {}
    point(const point& x) {
        bit_form = new bool[x.length];
        real_form = new long double[x.length/x.nrBiti];
        nrBiti = x.nrBiti;
        length = x.length;
        dim = x.dim;

        memcpy(this->bit_form, x.bit_form, length);
        memcpy(this->real_form, x.real_form, length/nrBiti * sizeof(long double));
    }

    /*point(point&& x) {
        bit_form = x.bit_form;
        real_form = x.real_form;
        length = x.length;
        nrBiti = x.nrBiti;

        x.bit_form = NULL;
        x.real_form = NULL;
        x.nrBiti = x.length = 0;
    }*/

    ~point() {
        if (bit_form != NULL) delete[] bit_form; 
        if (real_form != NULL) delete[] real_form;

        bit_form = NULL;
        real_form = NULL;
        length = nrBiti = dim = 0;
    }

    void copy(const point& x) {
        if (bit_form != NULL) delete bit_form;
        if (real_form != NULL) delete real_form;

        bit_form = new bool[x.length];
        real_form = new long double[x.length/x.nrBiti];

        memcpy(bit_form, x.bit_form, x.length);
        memcpy(real_form, x.real_form, (x.length/x.nrBiti) * sizeof(long double));

        length = x.length;
        nrBiti = x.nrBiti;
        dim = x.dim;
    }

    void Initialise(const long double& a, const long double& b, const long double& epsilon, const int& dimensions) {
        real_form = new long double[dimensions];
        nrBiti = log2Superior((b-a)/epsilon);
        length = nrBiti * dimensions;
        dim = dimensions;
        bit_form = new bool[nrBiti*dimensions];
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

//RNG
void randomCandidate(point& vc, const long double& a, const long double& b, const long double epsilon, const int& dimensions);

//Hill Climbing
long double hillClimbing(const int& dimensions, long double (*function)(const point& x), const long double& a, const long double& b, const long double& epsilon,  void (*Improve)(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x)));

//Other (2/2)
void CustomPrint(point& x, long double fx);

int main() {
    cout << setprecision(7) << fixed;
    //cout<<hillClimbing(30, Dejong, -5.12, 5.12, 0.00001, BestImprovement)<<'\n';
    cout<<hillClimbing(30, Schwefel, -500, 500, 0.00001, BestImprovement)<<'\n';
    //cout<<hillClimbing(30, Rastrigin, -5.12, 5.12, 0.00001, BestImprovement)<<'\n';
    //cout<<hillClimbing(5, Michalewicz, 0, M_PI, 0.00001, BestImprovement)<<'\n';
    return 0;
}

long double hillClimbing(const int& dimensions, long double (*function)(const point& x), const long double& a, const long double& b, const long double& epsilon,  void (*Improve)(const point& x, point& destination, const int& a, const int& b, long double (*function)(const point& x))) {
    int t = 0;
    point best, vc, vn;
    bool local;

    randomCandidate(best, a, b, epsilon, dimensions);
    do {
        local = false;
        randomCandidate(vc, a, b, epsilon, dimensions);
        do {
            Improve(vc, vn, a, b, function);
            /*cout<<"\nFor VC:\n";
            CustomPrint(vc, function(vc));
            cout<<"For VN:\n";
            CustomPrint(vn, function(vn));*/

            if (function(vn) < function(vc)) {
                vc.copy(vn);
            }
            else
                local = true;
            vn.~point();
        }while(!local);
        t++;
        if (function(vc) < function(best))
            best.copy(vc);

        vc.~point();
    }while(t < MAX);

    return function(best);
}

int log2Superior(const long double& x) {
    int rez = 0;
    while (x > 1<<rez) rez++;
    return rez;
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
    mt19937 mt(random_device{}());
    uniform_int_distribution coin{0, 1};

    vc.Initialise(a, b, epsilon, dimensions);

    for (int i = 0; i < vc.length; i++) {
        vc.bit_form[i] = coin(mt);
    }

    vc.UpdateReal(a, b);
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
