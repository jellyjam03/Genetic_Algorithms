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
#include <set>

static constexpr double epsilon = 1e-5;
static constexpr size_t MAX_VALUES = 30; // MAX_VALUES
static constexpr size_t MAX_BITS = MAX_VALUES * 32; // MAX_BITS

using ValArray = std::array<double, MAX_VALUES>;
using BitArray = std::array<bool, MAX_BITS>;

double deJong(const ValArray& x, size_t dims) {
	double sum = 0;
	for (size_t i = 0; i < dims; ++i) {
		sum += x[i] * x[i];
	}
	return sum;
}

double schwefel(const ValArray& x, size_t dims) {
	double sum = 0;
	for (size_t i = 0; i < dims; ++i) {
		sum += -x[i] * sin(sqrt(abs(x[i])));
	}
	return sum;
}

double rastrigin(const ValArray& x, size_t dims) {
	double sum = 0;
	for (size_t i = 0; i < dims; ++i) {
		sum += x[i] * x[i] - 10 * cos(2 * std::numbers::pi * x[i]);
	}
	return 10 * x.size() + sum;
}

double michalewicz(const ValArray& x, size_t dims) {
	double sum = 0;
	for (size_t i = 0; i < dims; ++i) {
		sum += sin(x[i]) * std::pow((sin((i + 1) * x[i] * x[i] / std::numbers::pi)), 20);
	}
	return -sum;
}

struct FunctionData {
	double rangeStart, rangeEnd, range;
	unsigned valueBitLength, numbersRepresented;

	FunctionData() = default;

	constexpr FunctionData(double rangeStart_, double rangeEnd_) :
		rangeStart(rangeStart_), rangeEnd(rangeEnd_),
		range(rangeEnd_ - rangeStart_), valueBitLength(ceilLog2(unsigned(range / epsilon))),
		numbersRepresented(cpow2(valueBitLength)) {}

	ValArray convertArray(const BitArray& bits, size_t dimensions) const {
		ValArray result{};
		for (size_t i = 0; i < dimensions; ++i) {
			result[i] = convert(bits, i * valueBitLength);
		}
		return result;
	}

	double convert(const BitArray& bits, size_t from) const {
		int integerConversion = 0;
		for (size_t bitIndex = from; bitIndex < from + valueBitLength; ++bitIndex) {
			integerConversion *= 2;
			integerConversion += bits[bitIndex];
		}
		return integerConversion * range / numbersRepresented + rangeStart;
	}

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

enum class FuncID {
	dejong = 0, schwefel, rastrigin, michalewicz, FUNC_COUNT
};

static const std::array<double(*)(const ValArray&, size_t), (size_t)FuncID::FUNC_COUNT> funcs = {
	deJong, schwefel, rastrigin, michalewicz
};

static constexpr std::array<const char*, (size_t)FuncID::FUNC_COUNT> funcName = {
	"De Jong's Function", "Schwefel's Function", "Rastrigin's Function", "Michalewicz's function"
};

static constexpr std::array<FunctionData, (size_t)FuncID::FUNC_COUNT> FDs = {
	FunctionData(-5.12, 5.12 + epsilon),
	FunctionData(-500, 500 + epsilon),
	FunctionData(-5.12, 5.12 + epsilon),
	FunctionData(0, std::numbers::pi + epsilon)
};

struct Function {
	Function() = delete;

	Function(size_t dims, FuncID fn) :
		fd(FDs[(size_t)fn]), dims(dims), fId(fn) {}

	double operator() (const ValArray& values) const {
		return funcs[(size_t)fId](values, dims);
	}

	const char* name() const {
		return funcName[(size_t)fId];
	}

	const std::reference_wrapper<const FunctionData> fd;
	const size_t dims;
	const FuncID fId;
};

struct Candidate {
	Candidate() = delete;
	Candidate(std::mt19937& rng, size_t dims_, FuncID funcName) : 
		dims(dims_), fId(funcName), fd(FDs[(size_t)fId]), _bits(random_bits(rng, dims_ * fd.get().valueBitLength)),
		_values(fd.get().convertArray(_bits, dims_)), eval(0.) {
		compute();
	}

	Candidate(const BitArray& bits, size_t dims_, FuncID funcName) : 
		dims(dims_), fId(funcName), fd(FDs[(size_t)fId]), _bits(bits), _values(fd.get().convertArray(_bits, dims_)), eval(0.) {
		compute();
	}

	void bitFlip(size_t index) {
		if (index >= dims * fd.get().valueBitLength) {
			throw std::invalid_argument("Bad index for bitstring");
		}
		_bits[index] = !_bits[index];
		size_t valueIndex = index / fd.get().valueBitLength;
		_values[valueIndex] = fd.get().convert(_bits, valueIndex * fd.get().valueBitLength);
		compute();
	}

	void rebuildValues() {
		_values = fd.get().convertArray(_bits, dims);
		compute();
	}

	void compute() {
		eval = funcs[(size_t)fId](_values, dims);
	}

	double f() const {
		return eval;
	}

	const ValArray& values() const {
		return _values;
	}

	const BitArray& bits() const {
		return _bits;
	}

	ValArray& values() {
		return _values;
	}

	BitArray& bits() {
		return _bits;
	}

	size_t bitsSize() const {
		return dims * fd.get().valueBitLength;
	}

	friend bool operator < (const Candidate& a, const Candidate& b) {
		return a.eval < b.eval || (a.eval == b.eval && a._bits < b._bits); 
	}

	friend void swap(Candidate& a, Candidate& b) {
		Candidate c(a);
		a = b;
		b = c;
	}

	size_t dimensions() const {
		return dims;
	}

	FuncID functionId() const {
		return fId;
	}

	const FunctionData& functionData() const {
		return fd.get();
	}

private:
	size_t dims;
	FuncID fId;
	std::reference_wrapper<const FunctionData> fd;
	BitArray _bits;
	ValArray _values;
	double eval;

	BitArray random_bits(std::mt19937& rng, size_t N = MAX_BITS) const {
		std::uniform_int_distribution<> bitGenerator{ 0, 1 };
		BitArray bits{};
		for (size_t i = 0; i < N; ++i) {
			bits[i] = bitGenerator(rng);
		}
		return bits;
	}
};

static std::vector<std::pair<double, size_t>> fitness(const std::vector<Candidate>& candidates, const Function& func) {
	std::vector<std::pair<double, size_t>> fitScore(candidates.size());
	size_t i = 0;
	for (const Candidate& data : candidates) {
		fitScore[i] = { func(data.values()), i };
		++i;
	}
	return fitScore;
}

static std::pair<Candidate, Candidate> crossover(const Candidate& p0, const Candidate& p1, size_t cuts, std::mt19937& rng) {
	if (cuts == 0) {
		return { p0, p1 };
	}
	if (cuts > p1.bitsSize()) {
		throw std::invalid_argument("Too many cuts");
	}
	size_t bs = p1.bitsSize();
	std::array<size_t, MAX_BITS> indices = {};
	std::iota(indices.begin(), indices.end(), 0);
	std::shuffle(indices.begin(), indices.end(), rng); // cuts will occur at first 'cuts' positions;
	std::sort(indices.begin(), indices.begin() + cuts);
	size_t cutIndex = 0;
	bool p0tob0 = true;
	std::array<const Candidate*, 2> p = { &p0, &p1 };
	std::array<BitArray, 2> b = {};
	for (size_t i = 0; i < p0.bitsSize(); ++i) {
		if (cutIndex < cuts && i == indices[cutIndex]) {
			cutIndex++;
			p0tob0 = !p0tob0;
		}
		b[0][i] = p[ p0tob0]->bits()[i];
		b[1][i] = p[!p0tob0]->bits()[i];
	}
	return { Candidate(b[0], p1.dimensions(), p1.functionId()), Candidate(b[1], p1.dimensions(), p1.functionId())};
}

static Candidate mutate(Candidate chromosome, size_t generationNumber, std::mt19937& rng) {
	std::uniform_real_distribution<> urd(0., 1.);
	double p = 1. / (4 * generationNumber);
	for (bool& bit : chromosome.bits()) {
		if (urd(rng) < p) {
			bit = !bit;
		}
	}
	chromosome.rebuildValues();
	return chromosome;
}

static std::vector<Candidate> nextGeneration(std::vector<Candidate>& generation, size_t generationNumber, const Function& func, std::mt19937& rng) {
	std::shuffle(generation.begin(), generation.end(), rng);
	auto fitScores = fitness(generation, func);
	// hashset to prevent duplicates
	std::set<Candidate> selected;
	constexpr size_t TOURNEY_GROUP = 5;
	if (generation.size() % TOURNEY_GROUP != 0) {
		std::cerr << "Bad Tourney Group size\n";
		exit(1);
	}
	for (size_t i = 0; i < fitScores.size(); i += TOURNEY_GROUP) {
		std::sort(fitScores.begin() + i, fitScores.begin() + i + TOURNEY_GROUP);
		// for each TOURNEY_GROUP members, create 4 candidates (c1, c2, m1, m2) and maintain the parents
		size_t i1 = fitScores[i].second, i2 = fitScores[i + 1].second;
		auto [c1, c2] = crossover(generation[i1], generation[i2], 50, rng);
		Candidate m1 = mutate(c1, generationNumber, rng), m2 = mutate(c2, generationNumber, rng);
		selected.insert({ c1, c2, m1, m2, generation[i1], generation[i2] });
	}
	while (selected.size() < generation.size()) {
		selected.insert(Candidate(rng, generation[0].dimensions(), generation[0].functionId()));
	}
	std::vector<Candidate> nextGen(selected.begin(), std::next(selected.begin(), generation.size()));
	return nextGen;
}

static void printCandidates(const std::vector<Candidate>& cand) {
	std::cout << "Candidates:\n";
	for (const Candidate& c : cand) {
		for (size_t i = 0; i < c.dimensions(); ++i) {
			std::cout << c.values()[i] << ' ';
		}
		std::cout << c.f() << std::endl;
	}
	std::cout << std::endl;
}

static double geneticAlgorithm(const Function& func) {
	static std::random_device rd;
	static std::seed_seq sq = { rd(), rd(), rd(), rd() };
	static std::mt19937 rng(sq);
	bool static warmup = false;
	if (!warmup) {
		warmup = true;
		rng.discard(5000);
	}
	std::vector<Candidate> candidates; 
	constexpr size_t POP_SIZE = 100;
	candidates.reserve(POP_SIZE);
	for (size_t i = 0; i < POP_SIZE; ++i) {
		candidates.emplace_back(rng, func.dims, func.fId);
	}
	//printCandidates(candidates);
	auto bestf = [](const Candidate& a, const Candidate& b) -> bool {
		return a.f() < b.f();
	};
	double best = std::min_element(candidates.begin(), candidates.end(), bestf)->f();
	int attempts = 10;
	for (size_t i = 1; i <= 1000; ++i) {
		candidates = nextGeneration(candidates, i, func, rng);
		//printCandidates(candidates);
		double current = std::min_element(candidates.begin(), candidates.end(), bestf)->f();
		//std::cout << best << ' ' << current << '\n';
		if (current < best) {
			best = current;
		}
	}

	return best;
}

int main() {
	std::array<Function, 12> funcs = {
		Function(5,  FuncID::dejong),
		Function(10, FuncID::dejong),
		Function(30, FuncID::dejong),
		Function(5,  FuncID::schwefel),
		Function(10, FuncID::schwefel),
		Function(30, FuncID::schwefel),
		Function(5,  FuncID::rastrigin),
		Function(10, FuncID::rastrigin),
		Function(30, FuncID::rastrigin),
		Function(5,  FuncID::michalewicz),
		Function(10, FuncID::michalewicz),
		Function(30, FuncID::michalewicz),
	};
	double avg[4] = { 0 };
	for (int i = 0; i < 30; ++i) {
		std::cout << "loop " << i << std::endl;
		avg[0] += geneticAlgorithm(funcs[2]);
		avg[1] += geneticAlgorithm(funcs[5]);
		avg[2] += geneticAlgorithm(funcs[8]);
		avg[3] += geneticAlgorithm(funcs[11]);
	}
	for (int i = 0; i < 4; ++i) {
		std::cout << avg[i] / 30. << std::endl;
	}

}