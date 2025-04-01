#include <mlgames/GenPopulation.h>

GenPopulation::GenPopulation(
        size_t population_size_, double mutation_rate_, const torch::nn::AnyModule &net)
{
    population_size = population_size_;
    mutation_rate = mutation_rate_;

    for (size_t i = 0; i < population_size; i++)
    {
        members.push_back(net);
    }
}
