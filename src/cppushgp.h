#ifndef CPPUSHGP_H
#define CPPUSHGP_H

#include "code.h"
#include "rng.h"

#include <cstddef>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

namespace cppush {

using ErcGenerator = std::function<Literal (RandomGenerator&)>;

struct PushGPConfig {
	std::vector<Instruction> instruction_set;
	std::vector<Literal> literal_set;
	std::vector<ErcGenerator> erc_generators;
	PushConfig push_config;
	int population_size = 500;
	int max_generations = 100;
	int initial_genome_size = 50;
};

struct Gene {
	enum class Type {
		Instruction, Literal, Close
	};
	Type type;
	std::optional<Code> code = std::nullopt;
};
using Genome = std::vector<Gene>;

class PushGP {
public:
	PushGP(PushGPConfig config);
	PushGP(PushGPConfig config, unsigned seed);

	Program get_best();

protected:
	virtual std::size_t num_fitness_cases() const = 0;
	virtual double evaluate(Program individual, std::size_t fitness_case_index) const = 0;

	void train(int gens); // throws if no fitness cases loaded
	void evaluate_population();

	PushGPConfig config;
	RandomGenerator rng;
	int generation;
	std::vector<Genome> population;
	std::vector<std::vector<double>> scores;
	double best_score;
	Program best_individual;

private:
	void init();
	Genome random_genome(int size);
	Gene random_gene();
	Code genome_to_code(Genome genome) const;
};

} // namespace cppush

#endif // CPPUSHGP_H
