#pragma once

#include <array>
#include <vector>
#include "NetDNA.h"

class PopulationDNA
{
public:

    PopulationDNA(
            size_t population_size_, double mutation_rate_, double mutation_rate_lower_,
            const NetDNA &net);
    std::vector<NetDNA> members{};
    void apply_mutations(std::array<size_t, 2> best_indexes);

private:

    size_t population_size{};
    double mutation_rate{};
    double mutation_rate_lower{};

    static void crossover(const NetDNA &parent1, const NetDNA &parent2, NetDNA &child);
    void mutate(NetDNA &net) const;
};
