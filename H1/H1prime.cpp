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

static constexpr int N = 5;

int h1primeFunction(const std::array<bool, N>& xBits) {
	constexpr int rangeStart = 0, rangeEnd = 32;
	constexpr size_t N = 5;
	int x = 0;
	for (size_t i = 0; i < N; ++i) {
		x *= 2;
		x += xBits[i];
	}
	return x * x * x - 60 * x * x + 900 * x + 100;
}

struct Function {
	Function(std::function<int(const std::array<bool, N>&)> func_, const std::string& name_) :
		func(func_), name(name_) {}

	virtual ~Function() = default;

	int operator() (const std::array<bool, N>& bits) const {
		return func(bits);
	}

	std::string name;

private:
	std::function<int(const std::array<bool, N>&)> func;
};


int bestImprovement(std::array<bool, N> bits, const Function& eval, std::mt19937&) {
	int bestValue = eval(bits);
	while (true) {
		int bestCurrent = bestValue;
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			bits[i] = !bits[i];
			int current = eval(bits);
			if (current > bestCurrent) {
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

int firstImprovement(std::array<bool, N> bits, const Function& eval, std::mt19937& rng) {
	std::array<size_t, N> indexes = {};
	std::iota(indexes.begin(), indexes.end(), 0);
	std::shuffle(indexes.begin(), indexes.end(), rng);
	int bestValue = eval(bits);
	while (true) {
		size_t bestIndex = N;
		for (size_t i = 0; i < N; ++i) {
			bits[indexes[i]] = !bits[indexes[i]];
			int current = eval(bits);
			if (current > bestValue) {
				bestIndex = indexes[i];
				bestValue = current;
				break;
			}
			bits[indexes[i]] = !bits[indexes[i]];
		}
		if (bestIndex == N) {
			return bestValue;
		}
		bits[bestIndex] = !bits[bestIndex];
	}
}

std::array<bool, N> random_bits(std::mt19937& rng) {
	std::uniform_int_distribution<> bitGenerator{ 0, 1 };
	std::array<bool, N> bits = {};
	for (bool& bit : bits) {
		bit = bitGenerator(rng);
	}
	return bits;
}

using ImproveType = std::function<int(std::array<bool, N>, const Function&, std::mt19937&)>;

std::string hillClimbingWithType(const Function& func, const ImproveType& improve) {
	std::stringstream output;
	std::chrono::steady_clock timer;
	std::chrono::steady_clock::time_point begin = timer.now();
	output << func.name << ',';
	std::random_device rd;
	std::seed_seq sq = { rd(), rd(), rd(), rd() };
	std::mt19937 rng(sq);
	rng.discard(5000);
	int resultValue = INT_MIN;
	constexpr int STEPS = 1000;
	for (int _ = 0; _ < STEPS; ++_) {
		int bestValue = improve(random_bits(rng), func, rng);
		resultValue = std::max(resultValue, bestValue);
	}
	output << resultValue << ',';
	std::chrono::steady_clock::time_point end = timer.now();
	output << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	return std::string(output.str());
}

void h1_prime_main() {
	std::ofstream fout("H1prime.csv", std::ios::app);
	constexpr int REPETITIONS = 50;
	Function func(h1primeFunction, "H1' Function");
	for (int _ = 0; _ < REPETITIONS; ++_) {
		fout << hillClimbingWithType(func, bestImprovement) << '\n';
		fout << hillClimbingWithType(func, firstImprovement) << '\n';
	}
}