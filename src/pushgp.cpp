#include "cppush/pushgp.hpp"

namespace cppush {

struct Pushgp::Individual {
	
};

Pushgp::Pushgp() {}

void Pushgp::fit(std::vector<std::vector<double>> X, std::vector<double> y) {
	// initialize population
	population.clear();

	// while max gens not reached:
	for (int gen = 0; gen < 100; ++gen) {
		// evaluate population
		// update best_seen
		// break if early stop condition reached
		// produce next generation
	}
}

double Pushgp::predict(std::vector<double> x) {}

} // namespace cppush