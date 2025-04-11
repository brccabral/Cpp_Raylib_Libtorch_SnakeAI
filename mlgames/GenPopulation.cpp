#include <random>
#include <mlgames/GenPopulation.h>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> mutation_dist(-0.1, 0.1);

GenPopulation::GenPopulation(
        size_t population_size_, double mutation_rate_, double mutation_rate_lower_,
        const LinearGen &net)
{
    population_size = population_size_;
    mutation_rate = mutation_rate_;
    mutation_rate_lower = mutation_rate_lower_;

    for (size_t i = 0; i < population_size; i++)
    {
        auto clone = net->clone();
        if (i > 0)
        {
            clone->mutate(mutation_rate);
        }
        members.push_back(clone);
    }
}

void GenPopulation::apply_mutations(std::array<size_t, 2> best_indexes)
{
    std::vector<LinearGen> new_members;
    new_members.reserve(members.size());
    // leave the best untouched
    new_members.push_back(members[best_indexes[0]]);

    for (size_t i = 1; i < population_size; i++)
    {
        auto child = members[best_indexes[0]]->clone();
        crossover(members[best_indexes[0]], members[best_indexes[1]], child);
        child->mutate(mutation_rate);
        new_members.push_back(child);
    }

    mutation_rate *= 0.99;
    mutation_rate = std::max(mutation_rate_lower, mutation_rate);

    members = std::move(new_members);
}

void GenPopulation::crossover(
        const LinearGen &parent1, const LinearGen &parent2, const LinearGen &child)
{
    for (size_t h = 0; h < parent1->hidden_sizes.size(); ++h)
    {
        const auto h1_weights = parent1->hidden_layers[h]->weight.clone();
        const auto h2_weights = parent2->hidden_layers[h]->weight.clone();
        int split = std::uniform_int_distribution<int>(0, h1_weights.size(1) - 1)(gen);
        child->hidden_layers[h]->weight.slice(1, 0, split) = h1_weights.slice(1, 0, split);
        child->hidden_layers[h]->weight.slice(1, split, h1_weights.size(1)) =
                h2_weights.slice(1, split, h2_weights.size(1));
    }

    const auto o1_weights = parent1->output->weight.clone();
    const auto o2_weights = parent2->output->weight.clone();
    int split = std::uniform_int_distribution<int>(0, o1_weights.size(1) - 1)(gen);
    child->output->weight.slice(1, 0, split) = o2_weights.slice(1, 0, split);
    child->output->weight.slice(1, split, o1_weights.size(1)) =
            o2_weights.slice(1, split, o2_weights.size(1));
}
