#pragma once
#include <torch/torch.h>
#include "MultiLayer.h"

class GenPopulation
{
public:

    GenPopulation(size_t population_size_, double mutation_rate_, std::shared_ptr<MultiLayer> net);
    std::vector<std::shared_ptr<MultiLayer>> members;

    void apply_mutations(std::array<size_t, 2> best_indexes);

private:

    size_t population_size;
    double mutation_rate;

    static void crossover(
            const std::shared_ptr<MultiLayer> &parent1, const std::shared_ptr<MultiLayer> &parent2,
            const std::shared_ptr<MultiLayer> &child);
    void mutate(const std::shared_ptr<MultiLayer> &net);
};
