#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng(rd());

extern double default_probability_generator() {
    return std::generate_canonical<double, 10>(rng);
}

ProbabilityGenerator probability_generator = default_probability_generator;

#endif /* HELPERS_HPP_ */
