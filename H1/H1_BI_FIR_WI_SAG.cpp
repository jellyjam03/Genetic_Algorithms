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
#include <bitset>
#include <array>
#include <numbers>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <string_view>
#include <algorithm>

static constexpr double epsilon = 1e-5;

struct FunctionData {
	double rangeStart, rangeEnd, range;
	unsigned valueBitLength, numbersRepresented;

	constexpr FunctionData(double rangeStart_, double rangeEnd_) :
		rangeStart(rangeStart_), rangeEnd(rangeEnd_),
		range(rangeEnd_ - rangeStart_), valueBitLength(ceilLog2(unsigned(range / epsilon))), 
		numbersRepresented(cpow2(valueBitLength)) {}

private:
	constexpr unsigned ceilLog2(unsigned n) const {
		if (n < 2) {
			return n;
		}
		return 1 + ceilLog2(n / 2);
	}

	constexpr unsigned cpow2(const unsigned pow) const {
		if (pow == 0) {
			return 1;
		}
		if (pow == 1) {
			return 2;
		}
		unsigned val = cpow2(pow / 2);
		if (pow % 2 == 1) {
			return val * val * 2;
		}
		return val * val;
	}
};

template<size_t N>
double deJong(const std::array<bool, N>& xBits) {
	constexpr double rangeStart = -5.12, rangeEnd = 5.12 + epsilon;
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if constexpr (N % fd.valueBitLength != 0) {
		throw std::invalid_argument("Bad length of bitset");
	}
	constexpr size_t xLen = N / fd.valueBitLength;
	double sum = 0;
	for (size_t i = 0; i <= N - fd.valueBitLength; i += fd.valueBitLength) {
		int xPosition = 0;
		for (size_t j = 1; j <= fd.valueBitLength; ++j) {
			xPosition *= 2;
			xPosition += xBits[i + fd.valueBitLength - j];
		}
		double x = xPosition * fd.range / fd.numbersRepresented + rangeStart;
		sum += x * x;
	}
	return sum;
}

template<size_t N>
double schwefel(const std::array<bool, N>& xBits) {
	constexpr double rangeStart = -500., rangeEnd = 500 + epsilon;
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if constexpr (N % fd.valueBitLength != 0) {
		throw std::invalid_argument("Bad length of bitset");
	}
	constexpr size_t xLen = N / fd.valueBitLength;
	double sum = 0;
	for (size_t i = 0; i <= N - fd.valueBitLength; i += fd.valueBitLength) {
		int xPosition = 0;
		for (size_t j = 1; j <= fd.valueBitLength; ++j) {
			xPosition *= 2;
			xPosition += xBits[i + fd.valueBitLength - j];
		}
		double x = xPosition * fd.range / fd.numbersRepresented + rangeStart;
		sum += -x * sin(sqrt(abs(x)));
	}
	return sum;
}

template<size_t N>
double rastrigin(const std::array<bool, N>& xBits) {
	constexpr double rangeStart = -5.12, rangeEnd = 5.12 + epsilon;
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if constexpr (N % fd.valueBitLength != 0) {
		throw std::invalid_argument("Bad length of bitset");
	}
	constexpr size_t xLen = N / fd.valueBitLength;
	double sum = 0;
	for (size_t i = 0; i <= N - fd.valueBitLength; i += fd.valueBitLength) {
		int xPosition = 0;
		for (size_t j = 1; j <= fd.valueBitLength; ++j) {
			xPosition *= 2;
			xPosition += xBits[i + fd.valueBitLength - j];
		}
		double x = xPosition * fd.range / fd.numbersRepresented + rangeStart;
		sum += x * x - 10 * cos(2 * std::numbers::pi * x);
	}
	return 10 * xLen + sum;
}

template<size_t N>
double michalewicz(const std::array<bool, N>& xBits) {
	constexpr double rangeStart = 0, rangeEnd = std::numbers::pi + epsilon;
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if constexpr (N % fd.valueBitLength != 0) {
		throw std::invalid_argument("Bad length of bitset");
	}
	constexpr size_t xLen = N / fd.valueBitLength;
	double sum = 0;
	for (size_t i = 0, k = 1; i <= N - fd.valueBitLength; i += fd.valueBitLength, ++k) {
		int xPosition = 0;
		for (size_t j = 1; j <= fd.valueBitLength; ++j) {
			xPosition *= 2;
			xPosition += xBits[i + fd.valueBitLength - j];
		}
		double x = xPosition * fd.range / fd.numbersRepresented + rangeStart;
		sum += sin(x) * std::pow((sin(k * x * x / std::numbers::pi)), 20) ;
	}
	return -sum;
}

struct GenericFunction {
	GenericFunction(const std::string& name_, int dims_) : name(name_), dimensions(dims_) {}
	std::string name;
	int dimensions;
	virtual ~GenericFunction() = 0 {}
};

template<size_t N>
struct Function : GenericFunction {
	Function(std::function<double(const std::array<bool, N>&)> func_, const std::string& name_, int dims_) :
		func(func_), GenericFunction(name_, dims_) {}

	virtual ~Function() = default;

	double operator() (const std::array<bool, N>& bits) const {
		return func(bits);
	}

private:
	std::function<double(const std::array<bool, N>&)> func;
};


template<unsigned DIMENSIONS>
void addWithDimension(std::vector<std::unique_ptr<GenericFunction>>& funcs) {
	constexpr FunctionData dejongFD = FunctionData(-5.12, 5.12 + epsilon),
		schwefelFD = FunctionData(-500, 500 + epsilon),
		rastriginFD = FunctionData(-5.12, 5.12 + epsilon),
		michalewiczFD = FunctionData(0, std::numbers::pi + epsilon);
	funcs.push_back(
		std::make_unique<Function<dejongFD.valueBitLength * DIMENSIONS>>(deJong<dejongFD.valueBitLength * DIMENSIONS>, "De Jong's function", DIMENSIONS)
	);
	funcs.push_back(
		std::make_unique<Function<schwefelFD.valueBitLength * DIMENSIONS>>(schwefel<schwefelFD.valueBitLength * DIMENSIONS>, "Schwefel's function", DIMENSIONS)
	);
	funcs.push_back(
		std::make_unique<Function<rastriginFD.valueBitLength * DIMENSIONS>>(rastrigin<rastriginFD.valueBitLength * DIMENSIONS>, "Rastrigin's function", DIMENSIONS)
	);
	funcs.push_back(
		std::make_unique<Function<michalewiczFD.valueBitLength * DIMENSIONS>>(michalewicz<michalewiczFD.valueBitLength * DIMENSIONS>, "Michalewicz's function", DIMENSIONS)
	);
}

template<size_t N>
double bestImprovement(std::array<bool, N> bits, const Function<N>* eval, std::mt19937&) {
	double bestValue = eval->operator()(bits);
	while (true) {
		double bestCurrent = bestValue;
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			bits[i] = !bits[i];
			double current = eval->operator()(bits);
			if (current < bestCurrent) {
				bestIndex = i;
				bestCurrent = current;
			}
			bits[i] = !bits[i];
		}
		if (bestIndex == N) {
			return bestValue;
		}
		bits[bestIndex] = !bits[bestIndex];
		bestValue = bestCurrent;
	}
}

template<size_t N>
double firstImprovement(std::array<bool, N> bits, const Function<N>* eval, std::mt19937& rng) {
	std::array<size_t, N> indexes;
	std::iota(indexes.begin(), indexes.end(), 0);
	double bestValue = eval->operator()(bits);
	while (true) {
		std::shuffle(indexes.begin(), indexes.end(), rng);
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			bits[indexes[i]] = !bits[indexes[i]];
			double current = eval->operator()(bits);
			if (current < bestValue) {
				bestIndex = indexes[i];
				bestValue = current;
				break;
			}
			bits[indexes[i]] = !bits[indexes[i]];
		}
		if (bestIndex == N) {
			return bestValue;
		}
	}
}

template<size_t N>
double worstImprovement(std::array<bool, N> bits, const Function<N>* eval, std::mt19937&) {
	double bestValue = eval->operator()(bits);
	while (true) {
		double worstCurrent = DBL_MIN;
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			bits[i] = !bits[i];
			double current = eval->operator()(bits);
			if (worstCurrent < current && current < bestValue) {
				bestIndex = i;
				worstCurrent = current;
			}
			bits[i] = !bits[i];
		}
		if (bestIndex == N) {
			return bestValue;
		}
		bits[bestIndex] = !bits[bestIndex];
		bestValue = worstCurrent;
	}
}

template<size_t N>
std::array<bool, N> random_bits(std::mt19937& rng) {
	std::uniform_int_distribution<> bitGenerator{ 0, 1 };
	std::array<bool, N> bits;
	for (bool& bit : bits) {
		bit = bitGenerator(rng);
	}
	return bits;
}

template<size_t N>
using ImproveType = std::function<double(std::array<bool, N>, const Function<N>*, std::mt19937&)>;

template<size_t N>
std::string hillClimbingWithType(Function<N>* func, ImproveType<N> improve) {
	std::stringstream output;
	output << std::fixed << std::setprecision(5);
	std::chrono::steady_clock timer;
	std::chrono::steady_clock::time_point begin = timer.now();
	output << func->name << ',' << func->dimensions << ',';
	std::random_device rd;
	std::seed_seq sq = { rd(), rd(), rd(), rd() };
	std::mt19937 rng(sq);
	rng.discard(5000);
	double resultValue = DBL_MAX;
	constexpr int STEPS = 1000;
	for (int _ = 0; _ < STEPS; ++_) {
		double bestValue = improve(random_bits<N>(rng), func, rng);
		resultValue = std::min(resultValue, bestValue);
	}
	output << resultValue << ',';
	std::chrono::steady_clock::time_point end = timer.now();
	output << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << ',';
	return std::string(output.str());
}

// https://www.scielo.org.mx/pdf/cys/v21n3/1405-5546-cys-21-03-00493.pdf

template<size_t A, size_t A_DIV, size_t B, size_t B_DIV>
double arithmethicGeometricCoolingSchedule(const double T, const size_t) {
	static_assert(A != 1 && B != 0 && A_DIV != 0 && B_DIV != 0);
	constexpr double a = 1.0 * A / A_DIV;
	constexpr double b = 1.0 * B / B_DIV;
	return T * a + b;
}

class Cooling_Schedule {
	std::function<double(const double, const size_t)> cooler;
	std::function<bool(const double)> checker;

public:
	Cooling_Schedule(const std::function<double(const double, const size_t)>& cool, const std::function<bool(const double)>& check) :
		cooler(cool), checker(check) {}

	double operator () (const double T, const size_t t) const {
		return cooler(T, t);
	}

	bool operator () (double T) const {
		return checker(T);
	}
};

template<size_t N>
std::string simulatedAnnealing(Function<N>* func, const Cooling_Schedule& coolingSchedule) {
	std::stringstream output;
	output << std::fixed << std::setprecision(5);
	std::chrono::steady_clock timer;
	std::chrono::steady_clock::time_point begin = timer.now();
	output << func->name << ',' << func->dimensions << ',';
	double temperature = 10000;
	std::random_device rd;
	std::seed_seq sq = { rd(), rd(), rd(), rd() };
	std::mt19937 rng(sq);
	std::uniform_real_distribution<> percentGenerator{ 0, 1 };
	std::uniform_int_distribution<size_t> indexGenerator{ 0, N - 1 };
	rng.discard(5000);
	std::array<bool, N> bits = random_bits<N>(rng);
	double bestAnswer = DBL_MAX;
	for (size_t iteration = 1; coolingSchedule(temperature); ++iteration, temperature = coolingSchedule(temperature, iteration)) {
		double currentAnswer = func->operator()(bits);
		bestAnswer = std::min(bestAnswer, currentAnswer);
		for (size_t i = 0; i < iteration; ++i) {
			size_t index = indexGenerator(rng);
			bits[index] = !bits[index];
			double newAnswer = func->operator()(bits);
			if (newAnswer < currentAnswer || percentGenerator(rng) < std::pow(std::numbers::e, -std::abs(newAnswer - currentAnswer) / temperature)) {
				currentAnswer = newAnswer;
				bestAnswer = std::min(bestAnswer, currentAnswer);
			}
			else {
				bits[index] = !bits[index];
			}
		}
	}
	output << bestAnswer << ',';
	std::chrono::steady_clock::time_point end = timer.now();
	output << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << ',';
	return output.str();
}

template<size_t N>
std::optional<std::string> runWithDimension(GenericFunction* genFunc) {
	if (auto func = dynamic_cast<Function<N>*>(genFunc)) {
		std::string output;
		output += hillClimbingWithType(func, (ImproveType<N>) bestImprovement<N>) + "Best improvement\n";
		std::cout << "Function<" << N << "> finished best improvement" << std::endl;
		output += hillClimbingWithType(func, (ImproveType<N>)firstImprovement<N>) + "First improvement\n";
		std::cout << func->name << "<" << N << "> finished first improvement\n";
		output += hillClimbingWithType(func, (ImproveType<N>)worstImprovement<N>) + "Worst improvement\n";
		std::cout << "Function<" << N << "> finished worst improvement" << std::endl;
		Cooling_Schedule cl(arithmethicGeometricCoolingSchedule<99, 100, 1, (size_t)1e7>, [](double T) -> bool { return T > 10 * epsilon; });
		output += simulatedAnnealing(func, cl) +"Simulated annealing\n";
		std::cout << "Function<" << N << "> finished simulated annealing" << std::endl;
		return std::optional<std::string>(output);
	}
	return std::nullopt;
}


std::string runAllTypes(GenericFunction* genFunc) {
	// function bit requirements
	constexpr size_t fbc[] = { 95, 100, 135, 190, 200, 270,570, 600, 810 };
	if (auto output = runWithDimension<fbc[0]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[1]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[2]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[3]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[4]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[5]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[6]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[7]>(genFunc)) return output.value();
	if (auto output = runWithDimension<fbc[8]>(genFunc)) return output.value();
	return "Bad Function\n";
}

int main() {
	std::ofstream fout("H1.csv", std::ios::app);
	std::vector<std::unique_ptr<GenericFunction>> funcs;
	addWithDimension<5>(funcs);
	addWithDimension<10>(funcs);
	addWithDimension<30>(funcs);
	constexpr int REPETITIONS = 10;
	constexpr int PARALLEL_THREADS = 4;
	auto threadRunner = [&fout, &funcs]() -> void {
		for (const auto& genFuncPtr : funcs) {
			fout << runAllTypes(genFuncPtr.get());
		}
	};
	for (int i = 0; i < REPETITIONS; ++i) {
		std::vector<std::future<void>> futures;
		for (int _ = 0; _ < PARALLEL_THREADS - 1; ++_) {
			futures.push_back(std::async(std::launch::async, threadRunner));
		}
		threadRunner();
		for (auto& ft : futures) {
			ft.get();
		}
		std::cout << "Finished repetition: " << i + 1 << std::endl;
	}
	return 0;
}