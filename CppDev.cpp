#include <iostream>
#include <functional>
#include <cmath>
#include <numeric>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>

constexpr double M_PI = 3.14159265358979323846;
constexpr double SMALLEST_INCREMENT = 1e-5;

double michalewicz(const std::vector<double>& x) {
    auto michalewicz1Dim = [](double x, int i) -> double {
        return sin(x) * std::pow(sin(x * x) * (i + 1) / M_PI, 20);
    };

    double sum = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += michalewicz1Dim(x[i], i);
    }
    return -sum;
}

double rastrigin(const std::vector<double>& x) {
    auto rastrigin1Dim = [](double x) -> double {
        return x * x - 10 * cos(2 * M_PI * x);
    };
    double sum = 0;
    for (double elem : x) {
        sum += rastrigin1Dim(elem);
    }
    return 10 * x.size() + sum;
}

double customFunc(const std::vector<double>& x) {
    auto customFunc1Dim = [](double x) -> double {
        return sin(2 * M_PI * x);
    };
    double sum = 0;
    for (double elem : x) {
        sum += customFunc1Dim(elem);
    }
    return sum;
}

double shubert(const std::vector<double>& x) {
    auto customFunc1Dim = [](double x) -> double {
        double sum = 0;
        for (int i = 1; i <= 5; ++i) {
            sum += i * cos((i + 1) * x + i);
        }
        return sum;
    };
    double sum = 0;
    for (double elem : x) {
        sum += customFunc1Dim(elem);
    }
    return sum;
}

struct Function {
private: Function();
    
public:
    Function(std::function<double(const std::vector<double>&)> func_, const std::string& name_) :
        func(func_), name(name_) {}

    std::function<double(const std::vector<double>&)> func;
    const std::string& name;

    double operator () (const std::vector<double>& x) {
        return func(x);
    }
};

void euristicGlobalMinimum(Function func, int dimensions, double rangeStart, double rangeEnd) {
    std::cout << func.name << " with " << dimensions << " dimensions and range [" << rangeStart << ", " << rangeEnd << "]\n";
    const double INCREMENTER[] = { 0, -SMALLEST_INCREMENT, SMALLEST_INCREMENT };
    const double ERROR = 1e-6;
    std::vector<double> bestX(dimensions);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> urd(rangeStart, rangeEnd + SMALLEST_INCREMENT);

    std::vector<int> divOffsets(dimensions);
    divOffsets[0] = 1;

    for (size_t i = 1; i < dimensions; ++i) {
        divOffsets[i] = divOffsets[i - 1] * 3;
    }
    for (double& elem : bestX) {
        elem = std::round(urd(rng) / SMALLEST_INCREMENT) * SMALLEST_INCREMENT;
    }
    double bestValue = func(bestX);
    while (true) {
        double bestNewValue = bestValue;
        int bestNeighbour = 0;
        for (int neighbour = 1, NEIGHBOURS = pow(3, dimensions); neighbour < NEIGHBOURS; ++neighbour) {
            std::vector<double> x(bestX);
            bool ok = true;
            for (size_t i = 0; i < x.size(); ++i) {
                x[i] += INCREMENTER[(int)(neighbour / divOffsets[i]) % 3];
                if (x[i] < rangeStart - ERROR || rangeEnd + ERROR < x[i]) {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                continue;
            }
            double newValue = func(x);
            if (newValue < bestNewValue) {
                bestNewValue = newValue;
                bestNeighbour = neighbour;
            }
        }
        if (bestNeighbour == 0) {
            break;
        }
        bestValue = bestNewValue;
        for (size_t i = 0; i < bestX.size(); ++i) {
            bestX[i] += INCREMENTER[(int)(bestNeighbour / divOffsets[i]) % 3];
        }
    }
    std::cout << "Found the value " << bestValue << " for x = {";
    for (double elem : bestX) {
        std::cout << elem << ", ";
    }
    std::cout << "\b\b} using the euristic method\n\n";
}

void deterministicGlobalMinimum(Function func, int dimensions, double rangeStart, double rangeEnd) {
    std::cout << func.name << " with " << dimensions << " dimensions and range [" << rangeStart << ", " << rangeEnd << "]\n";
    rangeEnd += SMALLEST_INCREMENT;
    size_t stepsPerDimension = std::ceil((rangeEnd - rangeStart) / SMALLEST_INCREMENT);
    if (stepsPerDimension >= pow(SIZE_MAX, 1. / dimensions)) {
        std::cout << "Step count over size_t limit, iteration is impossible and computation is unfeasable\n";
        std::cout << "Reduce dimension count or range size (current step count is " << pow(stepsPerDimension, dimensions) << ")\n\n";
        return;
    }
    std::vector<size_t> divOffsets(dimensions);
    divOffsets[0] = 1;
    for (size_t i = 1; i < dimensions; ++i) {
        divOffsets[i] = divOffsets[i - 1] * stepsPerDimension;
    }
    double bestValue = func(std::vector<double>(dimensions, rangeStart));
    size_t bestStep = 0;
    for (size_t step = 0, steps = pow(stepsPerDimension, dimensions); step < steps; ++step) {
        std::vector<double> x(dimensions, rangeStart);
        for (size_t i = 0; i < dimensions; ++i) {
            x[i] += SMALLEST_INCREMENT * (step / divOffsets[i] % stepsPerDimension);
        }
        double value = func(x);
        if (value < bestValue) {
            bestValue = value;
            bestStep = step;
        }
    }
    std::vector<double> bestX(dimensions, rangeStart);
    for (size_t i = 0; i < dimensions; ++i) {
        bestX[i] += SMALLEST_INCREMENT * (bestStep / divOffsets[i] % stepsPerDimension);
    }
    std::cout << "Found the value " << bestValue << " for x = {";
    for (double elem : bestX) {
        std::cout << elem << ", ";
    }
    std::cout << "\b\b} using the deterministic method\n\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(5);
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    euristicGlobalMinimum(Function(customFunc, "custom function"), 10, 0, 1);
    auto stop = high_resolution_clock::now();
    std::cout << "Function runtime was: " << duration_cast<seconds>(stop - start).count() << " seconds\n";
}