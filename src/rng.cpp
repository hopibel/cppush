#include "rng.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <random>

namespace cppush {

RandomGenerator::RandomGenerator() {
	std::array<unsigned, std::mt19937::state_size> random_data;
	std::random_device source;
	
	// include time in case random_device is deterministic
	random_data[0] = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::generate(random_data.begin()+1, random_data.end(), std::ref(source));
	std::seed_seq seeds(random_data.begin(), random_data.end());

	engine.seed(seeds);
}

int RandomGenerator::rand_int(int min, int max) {
	std::uniform_int_distribution dist(min, max);
	return dist(engine);
}

double RandomGenerator::rand_double(double min, double max) {
	std::uniform_real_distribution dist(min, max);
	return dist(engine);
}

} // namespace cppush
