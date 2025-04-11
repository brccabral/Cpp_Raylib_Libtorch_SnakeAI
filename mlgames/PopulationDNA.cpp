#include <random>
#include <mlgames/PopulationDNA.h>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> rand_weight(-1.0f, 1.0f);

PopulationDNA::PopulationDNA(
        size_t population_size_, double mutation_rate_, double mutation_rate_lower_,
        const NetDNA &net)
{
    population_size = population_size_;
    mutation_rate = mutation_rate_;
    mutation_rate_lower = mutation_rate_lower_;
    members.reserve(population_size_);

    for (size_t i = 0; i < population_size; i++)
    {
        auto clone = net.clone();
        if (i > 0)
        {
            clone.mutate(mutation_rate);
        }
        members.push_back(clone);
    }
}

void PopulationDNA::apply_mutations(std::array<size_t, 2> best_indexes)
{
    std::vector<NetDNA> new_members;
    new_members.reserve(members.size());
    // leave the best untouched
    new_members.push_back(members[best_indexes[0]]);

    for (size_t i = 1; i < population_size; i++)
    {
        auto child = members[best_indexes[0]].clone();
        crossover(members[best_indexes[0]], members[best_indexes[1]], child);
        child.mutate(mutation_rate);
        new_members.push_back(child);
    }

    mutation_rate *= 0.99;
    mutation_rate = std::max(mutation_rate_lower, mutation_rate);

    members = std::move(new_members);
}

void PopulationDNA::crossover(const NetDNA &parent1, const NetDNA &parent2, NetDNA &child)
{
    for (int h = 0; h < parent1.count_hidden; ++h)
    {
        const auto h1_weights = parent1.hidden[h].neurons;
        const auto h2_weights = parent2.hidden[h].neurons;
        const int split =
                std::uniform_int_distribution<int>(0, parent1.hidden[h].count_neurons - 1)(gen);
        for (auto n = 0; n < split; ++n)
        {
            child.hidden[h].neurons[n] = h1_weights[n];
        }
        for (auto n = split; n < parent2.hidden[h].count_neurons; ++n)
        {
            child.hidden[h].neurons[n] = h2_weights[n];
        }
    }
    const auto h1_weights = parent1.output.neurons;
    const auto h2_weights = parent2.output.neurons;
    const int split = std::uniform_int_distribution<int>(0, parent1.output.count_neurons - 1)(gen);
    for (auto n = 0; n < split; ++n)
    {
        child.output.neurons[n] = h1_weights[n];
    }
    for (auto n = split; n < parent2.output.count_neurons; ++n)
    {
        child.output.neurons[n] = h2_weights[n];
    }
}
