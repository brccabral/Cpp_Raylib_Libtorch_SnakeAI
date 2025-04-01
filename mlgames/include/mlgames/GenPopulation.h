#pragma once
#include <torch/torch.h>

class GenPopulation
{
public:

    GenPopulation(size_t population_size_, double mutation_rate_, const torch::nn::AnyModule &net);
    std::vector<torch::nn::AnyModule> members;

private:

    size_t population_size;
    double mutation_rate;
};
