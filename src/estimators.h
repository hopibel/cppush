#ifndef ESTIMATORS_H
#define ESTIMATORS_H

#include "env.h"
#include "cppushgp.h"

#include <cstddef>
#include <vector>

namespace cppush {

class FloatRegression : public PushGP {
public:
	using PushGP::PushGP;

	void fit(std::vector<double> inputs, std::vector<double> outputs, int gens);
	double predict(double input);

protected:
	virtual std::size_t num_fitness_cases() const override;
	virtual double evaluate(Program individual, std::size_t fitness_case_index) const override;

private:
	std::vector<double> inputs, outputs;
};

} // namespace cppush

#endif // ESTIMATORS_H
