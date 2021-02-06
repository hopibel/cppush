#include "estimators.h"

#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

namespace cppush {

void FloatRegression::fit(std::vector<double> inputs, std::vector<double> outputs, int gens) {
	if (inputs.size() != outputs.size()) {
		throw std::invalid_argument("FloatRegression::fit() inputs and outputs must be the same length");
	}

	this->inputs = inputs;
	this->outputs = outputs;
	train(gens);
}

// returns 0 if no output
double FloatRegression::predict(double input) {
	Env env;
	env.run(get_best(), { input });
	return env.get_outputs<double>()[0].value_or(0);
}

std::size_t FloatRegression::num_fitness_cases() const {
	return inputs.size();
}

double FloatRegression::evaluate(Program individual, std::size_t fitness_case_index) const {
	Env env;
	env.run(individual, { inputs[fitness_case_index] });
	auto result = env.get_outputs<double>()[0];
	if (result) {
		return std::abs(result.value() - outputs[fitness_case_index]);
	} else {
		return 1'000; // problem-specific "no output" penalty
	}
}

} // namespace cppush
