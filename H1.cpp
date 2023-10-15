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

template<size_t N, double rangeStart = -5.12, double rangeEnd = 5.12 + epsilon>
double deJong(const std::array<bool, N>& xBits) {
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if (N % fd.valueBitLength != 0) {
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

template<size_t N, double rangeStart = -500., double rangeEnd = 500 + epsilon>
double schwefel(const std::array<bool, N>& xBits) {
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if (N % fd.valueBitLength != 0) {
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

template<size_t N, double rangeStart = -5.12, double rangeEnd = 5.12 + epsilon>
double rastrigin(const std::array<bool, N>& xBits) {
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if (N % fd.valueBitLength != 0) {
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

template<size_t N, double rangeStart = 0., double rangeEnd = std::numbers::pi + epsilon>
double michalewicz(const std::array<bool, N>& xBits) {
	constexpr double rangeStart = 0, rangeEnd = std::numbers::pi + epsilon;
	constexpr FunctionData fd(rangeStart, rangeEnd);
	if (N % fd.valueBitLength != 0) {
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

	std::function<double(const std::array<bool, N>&)> func;
	virtual ~Function() = default;

	double operator() (const std::array<bool, N>& bits) {
		return func(bits);
	}
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
	//std::cout << dejongFD.valueBitLength * DIMENSIONS << ",\n"
	//	<< schwefelFD.valueBitLength * DIMENSIONS << ",\n"
	//	<< rastriginFD.valueBitLength * DIMENSIONS << ",\n"
	//	<< michalewiczFD.valueBitLength * DIMENSIONS << ",\n";
}

template<size_t N>
double bestImprovement(std::array<bool, N> bits, Function<N>* eval) {
	double bestValue = eval->operator()(bits);
	while (true) {
		double bestCurrent = bestValue;
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			std::array<bool, N> newBits = bits;
			newBits[i] = !newBits[i];
			double current = eval->operator()(newBits);
			if (current < bestCurrent) {
				bestIndex = i;
				bestCurrent = current;
			}
		}
		if (bestIndex == N) {
			return bestValue;
		}
		bits[bestIndex] = !bits[bestIndex];
		bestValue = bestCurrent;
	}
}

template<size_t N>
double firstImprovement(std::array<bool, N> bits, Function<N>* eval) {
	double bestValue = eval->operator()(bits);
	while (true) {
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			std::array<bool, N> newBits = bits;
			newBits[i] = !newBits[i];
			double current = eval->operator()(newBits);
			if (current < bestValue) {
				bestIndex = i;
				bestValue = current;
				break;
			}
		}
		if (bestIndex == N) {
			return bestValue;
		}
		bits[bestIndex] = !bits[bestIndex];
	}
}

template<size_t N>
double worstImprovement(std::array<bool, N> bits, Function<N>* eval) {
	double bestValue = eval->operator()(bits);
	while (true) {
		double worstCurrent = DBL_MIN;
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			std::array<bool, N> newBits = bits;
			newBits[i] = !newBits[i];
			double current = eval->operator()(newBits);
			if (worstCurrent < current && current < bestValue) {
				bestIndex = i;
				worstCurrent = current;
			}
		}
		if (bestIndex == N) {
			return bestValue;
		}
		bits[bestIndex] = !bits[bestIndex];
		bestValue = worstCurrent;
	}
}

template<size_t N>
std::array<bool, N> random_bits(std::uniform_int_distribution<>& bitGenerator, std::mt19937& rng) {
	std::array<bool, N> bits;
	for (bool& bit : bits) {
		bit = bitGenerator(rng);
	}
	return bits;
}

template<size_t N>
using ImproveType = std::function<double(std::array<bool, N>, Function<N>*)>;

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
	std::uniform_int_distribution<> bitGenerator{0, 1};
	rng.discard(5000);
	double resultValue = DBL_MAX;
	constexpr int STEPS = 500;
	for (int _ = 0; _ < STEPS; ++_) {
		double bestValue = improve(random_bits<N>(bitGenerator, rng), func);
		if (resultValue > bestValue) {
			resultValue = bestValue;
		}
	}
	output << resultValue << ',';
	std::chrono::steady_clock::time_point end = timer.now();
	output << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << ',';
	return std::string(output.str());
}

template<size_t N>
std::string simulatedAnnealing(Function<N>* func) {
	std::stringstream output;
	output << std::fixed << std::setprecision(5);
	std::chrono::steady_clock timer;
	std::chrono::steady_clock::time_point begin = timer.now();
	output << func->name << ',' << func->dimensions << ',';
	double temperature = 1;
	std::random_device rd;
	std::seed_seq sq = { rd(), rd(), rd(), rd() };
	std::mt19937 rng(sq);
	std::uniform_int_distribution<> bitGenerator{ 0, 1 };
	std::uniform_real_distribution<> percentGenerator{ 0, 1 };
	std::uniform_int_distribution<size_t> indexGenerator{ 0, N - 1 };
	rng.discard(5000);
	std::array<bool, N> bits = random_bits<N>(bitGenerator, rng);
	double bestAnswer = DBL_MAX;
	constexpr size_t MAX_ITER = N < 300 ? (size_t)5e5 : (size_t)2e6;
	for (size_t iteration = 0; iteration < MAX_ITER; ++iteration) {
		double currentAnswer = func->operator()(bits);
		bestAnswer = std::min(bestAnswer, currentAnswer);
		bool ok = true;
		while (ok) {
			size_t index = indexGenerator(rng);
			bits[index] = !bits[index];
			double newAnswer = func->operator()(bits);
			if (newAnswer < currentAnswer || percentGenerator(rng) < std::pow(std::numbers::e, -std::abs(newAnswer - currentAnswer) / temperature)) {
				currentAnswer = newAnswer;
				bestAnswer = std::min(bestAnswer, currentAnswer);
			}
			else {
				ok = false;
				bits[index] = !bits[index]; // restore previous bit
			}
		}
		temperature = (1 - (iteration + 1.) / MAX_ITER);
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
		std::cout << "Function<" << N << "> finished first improvement" << std::endl;
		output += hillClimbingWithType(func, (ImproveType<N>)worstImprovement<N>) + "Worst improvement\n";
		std::cout << "Function<" << N << "> finished worst improvement" << std::endl;
		output += simulatedAnnealing(func) + "Simulated annealing\n";
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
	std::ofstream fout("H1_hillClimbing.csv", std::ios::app);
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
	for (int _ = 0; _ < REPETITIONS; ++_) {
		std::vector<std::future<void>> futures;
		for (int _ = 0; _ < PARALLEL_THREADS - 1; ++_) {
			futures.push_back(std::async(std::launch::async, threadRunner));
		}
		threadRunner();
		for (auto& ft : futures) {
			ft.get();
		}
	}
}
