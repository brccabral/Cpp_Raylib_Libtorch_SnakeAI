#pragma once
#include <torch/torch.h>
#include <mlgames/LinearGen.h>


class GenPopulation
{
public:

    GenPopulation(
            size_t population_size_, double mutation_rate_, double mutation_rate_lower_,
            const LinearGen &net);
    std::vector<LinearGen> members{};

    void apply_mutations(std::array<size_t, 2> best_indexes);

private:

    size_t population_size{};
    double mutation_rate{};
    double mutation_rate_lower{};

    static void
    crossover(const LinearGen &parent1, const LinearGen &parent2, const LinearGen &child);
    void mutate(const LinearGen &net) const;
};
