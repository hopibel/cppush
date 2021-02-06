#ifndef RNG_H
#define RNG_H

#include <random>

namespace cppush {

class RandomGenerator {
public:
	RandomGenerator();
	RandomGenerator(unsigned seed) : engine(std::mt19937(seed)) {}

	int rand_int(int min, int max); // uniform int from [min, max]
	double rand_double(double min, double max); // uniform double from [min, max)

	std::mt19937 engine;
};

} // namespace cppush

#endif // RNG_H
