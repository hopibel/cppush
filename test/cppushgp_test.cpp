#include <catch2/catch.hpp>

#include "code.h"
#include "cppushgp.h"
#include "estimators.h"
#include "instruction_set.h"
#include "rng.h"

#include <iostream>
#include <utility>

// evolve the function x+1
TEST_CASE("Creating a PushGP instance for a problem") {
	auto instruction_set = cppush::register_core_by_name({
		"float_add", "float_from_integer"
	});
	cppush::register_n_inputs<1>(instruction_set);
	cppush::register_n_outputs<double, 1>(instruction_set);

	// no need for register_literals due to automatic type conversion
	std::vector<cppush::Literal> literal_set{
		1,
		1.0
	};

	// using ErcGenerator = std::function<Literal(RandomGenerator&)>;
	// we're using our own rng wrapper to simplify seeding
	std::vector<cppush::ErcGenerator> erc_generators{
		[](cppush::RandomGenerator& rng) {
			return cppush::Literal(rng.rand_double(1, 1)); }, // rand_int
	};

	// push_config
	cppush::PushConfig push_config;
	push_config.inputs_expected = 1;
	push_config.outputs_expected = 1;

	// pushgp_config
	cppush::PushGPConfig pushgp_config;
	pushgp_config.instruction_set = instruction_set;
	pushgp_config.literal_set = literal_set;
	pushgp_config.erc_generators = erc_generators;
	pushgp_config.initial_genome_size = 3;
	pushgp_config.population_size = 20;
	pushgp_config.push_config = push_config;

	// TODO(hopibel): construct rng and gene_factory in GP class instead
	// GeneFactory gene_factory{instruction_set, literal_set, erc_generators, rng};
	
	cppush::FloatRegression gp{pushgp_config, 0}; // optional rng seed

	// test cases
	std::vector<double> inputs, outputs;
	for (double i = -10; i < 10; i += 0.5) {
		inputs.push_back(i);
		outputs.push_back(i + 1);
	}

	// PushGP interface:
	// - getter for number of test cases
	// - std::vector<double> evaluate(Program)

	// run for a fixed number of generations
	// problem-specific. not part of interface
	// void fit(inputs, outputs, gens)
	gp.fit(inputs, outputs, 10); // throw if sizes unequal
	std::cout << gp.get_best().code << "\n";

	// some validation tests.
	// also problem-specific
	for (double i = -20; i < -10; i += 0.5) {
		REQUIRE(gp.predict(i) == i+1);
	}
}
