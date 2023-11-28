#include "Functions.h"

long double Dejong(const point& x) {
    int i;
    long double sum = 0;
    for (i = 0; i < x.dim; i++) {
        sum = sum + x.real_form[i] * x.real_form[i];
    }

    return sum;
}

long double Schwefel(const point& x) {
    long double sum = 418.9829 * x.dim;
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