#include "Point.h"

int log2Superior(const long double x) {
    int rez = 0;
    while (x > 1 << rez) rez++;
    return rez;
}

void randomCandidate(point& vc, const long double a, const long double b, const long double epsilon, const int& dimensions) {
    for (int i = 0; i < vc.length; i++) {
        vc.bit_form[i] = coin(mt);
    }

    vc.UpdateReal(a, b);
}

point::point(const long double a, const long double b, const long double epsilon, const int& dimensions) : point() {
    nrBiti = log2Superior((b - a) / epsilon);
    length = nrBiti * dimensions;
    dim = dimensions;
}

point::point(const point& x) {
    nrBiti = x.nrBiti;
    length = x.length;
    dim = x.dim;

    memcpy(this->bit_form, x.bit_form, length);
    memcpy(this->real_form, x.real_form, dim * sizeof(long double));
}

void point::copy(const point& x) {
    memcpy(bit_form, x.bit_form, x.length);
    memcpy(real_form, x.real_form, x.dim * sizeof(long double));

    length = x.length;
    nrBiti = x.nrBiti;
    dim = x.dim;
}

void point::UpdateReal(const long double a, const long double b) {
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

void point::BitFlip(const long double a, const long double b, const int poz) {
    unsigned long long int xDecimal = 0;
    int i = 0, offset = poz / nrBiti * nrBiti;
    bit_form[poz] = !bit_form[poz];

    for (i = 0; i < nrBiti; i++) {
        xDecimal *= 2;
        xDecimal += bit_form[offset + i];
    }
    real_form[poz / nrBiti] = a + xDecimal * (b - a) / ((1 << nrBiti) - 1);
}