#include "code.h"
#include "cppushgp.h"
#include "env.h"
#include "rng.h"

#include <limits>
#include <stdexcept>
#include <vector>

namespace cppush {

PushGP::PushGP(PushGPConfig config) : config(config) {
	init();
}

PushGP::PushGP(PushGPConfig config, unsigned seed) : config(config), rng(RandomGenerator(seed))
{
	init();
}

Program PushGP::get_best() {
	return best_individual;
}

void PushGP::init() {
	// validate config
	if (config.population_size < 1) {
		throw std::range_error("PushGPConfig: population_size must be > 0");
	} else if (config.max_generations < 1) {
		throw std::range_error("PushGPConfig: max_generations must be > 0");
	} else if (config.initial_genome_size < 1) {
		throw std::range_error("PushGPConfig: initial_genome_size must be > 0");
	}

	generation = 0;
	best_score = std::numeric_limits<double>::max();

	// initialize population
	for (int i = 0; i < config.population_size; ++i) {
		population.push_back(random_genome(config.initial_genome_size));
	}
}

// TODO: stub
void PushGP::train(int gens) {
	if (num_fitness_cases() == 0) {
		throw std::length_error("PushGP::train(): no fitness cases were loaded");
	}

	// initialize scores matrix
	if (scores.size() != num_fitness_cases()) {
		scores = std::vector<std::vector<double>>(
			num_fitness_cases(),
			std::vector<double>(
				config.population_size, std::numeric_limits<double>::max()
			)
		);
	}

	// evaluate initial population
	evaluate_population();

	for (int gen = 0; gen < gens; ++gen) {
		evaluate_population();
		// create next generation (selection + recombination)

		++generation;
	}
}

void PushGP::evaluate_population() {
	// TODO: parallelise
	for (unsigned individual = 0; individual < population.size(); ++individual) {
		Program prog{ genome_to_code(population[individual]), config.push_config };
		double total_error = 0;

		for (unsigned fitness_case = 0; fitness_case < num_fitness_cases(); ++fitness_case)
		{
			double score = evaluate(prog, fitness_case);
			total_error += score;
			scores[fitness_case][individual] = score;
		}

		// save best
		if (total_error < best_score) {
			best_score = total_error;
			best_individual = prog;
		}
	}
}

Genome PushGP::random_genome(int size) {
	Genome genome;
	genome.reserve(size);
	for (int i = 0; i < size; ++i) {
		genome.push_back(random_gene());
	}
	return genome;
}

Gene PushGP::random_gene() {
	double close_weight = 0;
	for (const auto& insn : config.instruction_set) {
		close_weight += insn.get_parens();
	}
	std::discrete_distribution<> dist({
		static_cast<double>(config.instruction_set.size()),
		static_cast<double>(config.literal_set.size()),
		static_cast<double>(config.erc_generators.size()),
		close_weight
	});

	int index;
	Gene g;
	switch (dist(rng.engine)) {
	case 0: // instruction
		index = rng.rand_int(0, config.instruction_set.size() - 1);
		g = Gene{Gene::Type::Instruction, config.instruction_set[index]};
		break;
	case 1: // literal
		index = rng.rand_int(0, config.literal_set.size() - 1);
		g = Gene{Gene::Type::Literal, config.literal_set[index]};
		break;
	case 2: // ERC
		index = rng.rand_int(0, config.erc_generators.size() - 1);
		g = Gene{Gene::Type::Literal, config.erc_generators[index](rng)};
		break;
	case 3: // close
		g = Gene{Gene::Type::Close};
		break;
	};

	return g;
}

// convert linear plushy genome to push tree structure
Code PushGP::genome_to_code(const Genome genome) const {
	std::vector<std::vector<Code>> stack;
	std::vector<Code> block; // current block being translated
	int queued_blocks = 0; // requested blocks queue

	for (Gene gene : genome) {
		switch (gene.type) {
		case Gene::Type::Instruction:
		{
			block.push_back(gene.code.value());
			const auto& insn = std::get<Instruction>(gene.code.value());

			if (insn.get_parens()) {
				queued_blocks += insn.get_parens() - 1; // open one immediately
				stack.push_back(block);
				block.clear();
			}
			break;
		}
		case Gene::Type::Close: // close current block
		{
			auto& previous_block = stack.back();
			previous_block.push_back(CodeList(block));
			if (queued_blocks) {
				--queued_blocks;
				block.clear();
			} else {
				block = previous_block;
				stack.pop_back();
			}
			break;
		}
		case Gene::Type::Literal:
			block.push_back(gene.code.value());
			break;
		};
	}

	// close unclosed blocks (balance parentheses)
	while (!stack.empty()) {
		auto& previous_block = stack.back();
		previous_block.push_back(CodeList(block));
		block = previous_block;
		stack.pop_back();
	}

	return CodeList(block);
}

} // namespace cppush
