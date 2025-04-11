#pragma once

#include <mlgames/NetGen.h>

class NetPopulation
{
public:

    NetPopulation(
            size_t population_size_, double mutation_rate_, double mutation_rate_lower_,
            const NetGen &net);
    std::vector<NetGen> members{};

    void apply_mutations(std::array<size_t, 2> best_indexes);

private:

    size_t population_size{};
    double mutation_rate{};
    double mutation_rate_lower{};

    static void crossover(const NetGen &parent1, const NetGen &parent2, NetGen &child);
};
