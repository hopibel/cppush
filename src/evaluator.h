#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "env.h"

namespace cppush {

class Evaluator {
public:
	virtual int count_cases() const = 0;
	virtual std::vector<double> evaluate(Program individual) const = 0;
};

class DatasetEvaluator : public Evaluator {
public:
	DatasetEvaluator(std::vector<double> X, std::vector<double> Y);

	int count_cases() const override;
	std::vector<double> evaluate(Program individual) const override;
};

} // namespace cppush

#endif // EVALUATOR_H
