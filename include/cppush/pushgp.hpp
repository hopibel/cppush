#ifndef PUSHGP_HPP
#define PUSHGP_HPP

#include "cppush/code.hpp"

#include <memory>
#include <vector>

namespace cppush {

class Pushgp {
public:
	// TODO: config struct
	Pushgp();

	void fit(std::vector<std::vector<double>> X, std::vector<double> y);
	double predict(std::vector<double> x);

private:
	struct Individual;

	std::vector<Individual> population;
};

} // namespace cppush

#endif // PUSHGP_HPP