#include <iostream>
#include <functional>
#include <cmath>
#include <numeric>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <future>

constexpr double M_PI = 3.14159265358979323846;
constexpr double SMALLEST_INCREMENT = 1e-5;

double michalewicz(const std::vector<double>& x) {
    auto michalewicz1Dim = [](double x, int i) -> double {
        return sin(x) * std::pow(sin((x * x) * i / M_PI), 20);
    };
    double sum = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += michalewicz1Dim(x[i], (int)i + 1);
    }
    return -sum;
}

double rastrigin(const std::vector<double>& x) {
    auto rastrigin1Dim = [](double sum, double x) -> double {
        return sum + x * x - 10 * cos(2 * M_PI * x);
    };
    return 10 * x.size() + std::accumulate(x.begin(), x.end(), 0.0, rastrigin1Dim);
}

double sphere(const std::vector<double>& x) {
    auto sphere1DimAux = [](double sum, double x) -> double {
        return sum + x * x;
    };
    return std::accumulate(x.begin(), x.end(), 0.0, sphere1DimAux);
}

double shubert(const std::vector<double>& x) {
    auto customFunc1Dim = [](double sumSoFar, double x) -> double {
        double sumCurrent = 0;
        for (int i = 1; i <= 5; ++i) {
            sumCurrent += i * cos((i + 1) * x + i);
        }
        return sumSoFar + sumCurrent;
    };
    return std::accumulate(x.begin(), x.end(), 0.0, customFunc1Dim);
}

double sumSquares(const std::vector<double>& x) {
    auto sumSquares1Dim = [](double x, int i) -> double {
        return i * x * x;
    };
    double sum = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += sumSquares1Dim(x[i], (int)i + 1);
    }
    return sum;
}

struct Function {
private: Function();
    
public:
    Function(std::function<double(const std::vector<double>&)> func_, const std::string& name_, double lo_, double hi_) :
        func(func_), name(name_), lo(lo_), hi(hi_) {}

    std::function<double(const std::vector<double>&)> func;
    std::string name;
    double lo;
    double hi; // inclusive range end

    double operator () (const std::vector<double>& x) const {
        return func(x);
    }
};

std::string heuristicGlobalMinimum(const Function& func, int dimensions) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    double rangeStart = func.lo;
    double rangeEnd = func.hi;
    std::stringstream prettyOutput;
    std::stringstream simpleOutput;
    simpleOutput << std::fixed << std::setprecision(5);
    simpleOutput << func.name << ',' << dimensions << ',' << rangeStart << ',' << rangeEnd << ',';
    const double INCREMENTER[] = { 0, -SMALLEST_INCREMENT, SMALLEST_INCREMENT };
    const double ERROR = 1e-6;
    std::vector<double> bestX(dimensions);

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_real_distribution<double> urd(rangeStart, rangeEnd + ERROR);

    std::vector<int> divOffsets(dimensions);
    divOffsets[0] = 1;

    for (size_t i = 1; i < dimensions; ++i) {
        divOffsets[i] = divOffsets[i - 1] * 3;
    }
    for (double& elem : bestX) {
        elem = std::round(urd(rng) / SMALLEST_INCREMENT) * SMALLEST_INCREMENT;
    }
    double bestValue = func(bestX);
    std::vector<double> x(bestX.size());
    int loop = 0;
    while (true) {
        double bestNewValue = bestValue;
        int bestNeighbour = 0;
        for (int neighbour = 1, NEIGHBOURS = pow(3, dimensions); neighbour < NEIGHBOURS; ++neighbour) {
            for (size_t elemIndex = 0; elemIndex < bestX.size(); ++elemIndex) {
                x[elemIndex] = bestX[elemIndex];
            }
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
        // around 250'000'000 steps
        if (dimensions == 10 && ++loop == 4000) {
            loop = 0;
            std::cout << "heuristic " << func.name << " still searching\n";
        }
        bestValue = bestNewValue;
        for (size_t i = 0; i < bestX.size(); ++i) {
            bestX[i] += INCREMENTER[(int)(bestNeighbour / divOffsets[i]) % 3];
        }
    }
    simpleOutput << bestValue << ",{";
    for (size_t i = 0; i < bestX.size() - 1; ++i) {
        simpleOutput << bestX[i] << ' ';
    }
    simpleOutput << bestX.back() << "},heuristic,";
    auto stop = high_resolution_clock::now();
    simpleOutput << duration_cast<milliseconds>(stop - start).count();
    return simpleOutput.str();
}

std::string deterministicGlobalMinimum(const Function& func, int dimensions) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    double rangeStart = func.lo;
    double rangeEnd = func.hi;
    std::stringstream simpleOutput;
    simpleOutput << std::fixed << std::setprecision(5);
    simpleOutput << func.name << ',' << dimensions << ',' << rangeStart << ',' << rangeEnd << ',';
    rangeEnd += SMALLEST_INCREMENT;
    size_t stepsPerDimension = std::ceil((rangeEnd - rangeStart) / SMALLEST_INCREMENT);
    if (stepsPerDimension >= pow(SIZE_MAX, 1. / dimensions)) {
        simpleOutput << "NULL,NULL,deterministic,NULL";
        return simpleOutput.str();
    }
    std::vector<size_t> divOffsets(dimensions);
    divOffsets[0] = 1;
    for (size_t i = 1; i < dimensions; ++i) {
        divOffsets[i] = divOffsets[i - 1] * stepsPerDimension;
    }
    std::vector<double> x(dimensions, rangeStart);
    double bestValue = func(x);
    size_t bestStep = 0;
    for (size_t step = 1, steps = pow(stepsPerDimension, dimensions); step < steps; ++step) {
        if (step % 250'000'000 == 0) {
            std::cout << "deterministic " << func.name << " is at " << (100.0 * step / steps) << "%\n";
        }
        std::fill(x.begin(), x.end(), rangeStart);
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
    simpleOutput << bestValue << ",{";
    for (size_t i = 0; i < bestX.size() - 1; ++i) {
        simpleOutput << bestX[i] << ' ';
    }
    simpleOutput << bestX.back() << "},deterministic,";
    auto stop = high_resolution_clock::now();
    simpleOutput << duration_cast<milliseconds>(stop - start).count();
    return simpleOutput.str();
}

int main() {
    Function michalewiczFunc(michalewicz, "Michalewicz's function", 1, 3);
    Function rastriginFunc(rastrigin, "Rastrigin's function", -1, 1);
    Function sumSquaresFunc(shubert, "Sum Squares function", -1, 1);
    Function sphereFunc(sphere, "Sphere function", -1, 1);
    Function funcs[] = {
        //michalewiczFunc,
        rastriginFunc,
        sumSquaresFunc,
        //sphereFunc
    };
    std::ofstream fout("output2.csv", std::ios_base::app);
    if (!fout.is_open()) {
        std::cerr << "output file could not be created";
        return 1;
    }
    constexpr int REPETITIONS = 5;
    for (int _ = 0; _ < REPETITIONS; ++_) {
        std::vector<std::future<std::string>> deterministicFutures;
        for (const Function& f : funcs) {
            deterministicFutures.emplace_back(std::async(std::launch::async, deterministicGlobalMinimum, f, 2));
            deterministicFutures.emplace_back(std::async(std::launch::async, deterministicGlobalMinimum, f, 2));
        }
        while (deterministicFutures.front().wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
            std::vector<std::future<std::string>> heuristicFutures;
            for (const Function& f : funcs) {
                fout << heuristicGlobalMinimum(f, 2) << std::endl;
                heuristicFutures.emplace_back(std::async(std::launch::async, heuristicGlobalMinimum, f, 10));
            }
            for (std::future<std::string>& ft : heuristicFutures) {
                fout << ft.get() << std::endl;
            }
        }
        for (std::future<std::string>& ft : deterministicFutures) {
            fout << ft.get() << std::endl;
        }
    }
}