#include <mlgames/GenPopulation.h>

GenPopulation::GenPopulation(
        size_t population_size_, double mutation_rate_, std::shared_ptr<MultiLayer> net)
{
    population_size = population_size_;
    mutation_rate = mutation_rate_;

    for (size_t i = 0; i < population_size; i++)
    {
        members.push_back(net);
    }
}

void GenPopulation::apply_mutations(std::array<size_t, 2> best_indexes)
{
    std::vector<std::shared_ptr<MultiLayer>> new_members;
    new_members.push_back(members[best_indexes[0]]);

    for (size_t i = 1; i < population_size; i++)
    {
        auto child = members[best_indexes[0]];
        crossover(members[best_indexes[0]], members[best_indexes[1]], child);
        new_members.push_back(child);
    }

    members = std::move(new_members);
}

void GenPopulation::crossover(
        const std::shared_ptr<MultiLayer> &parent1, const std::shared_ptr<MultiLayer> &parent2,
        const std::shared_ptr<MultiLayer> &child)
{
    {
        torch::NoGradGuard no_grad;
        for (size_t h = 0; h < parent1->hidden_sizes.size(); ++h)
        {
            const auto h1_weights = parent1->hidden_layers[h]->weight.clone();
            const auto h2_weights = parent2->hidden_layers[h]->weight.clone();
            int split = h1_weights.size(1) / 2;
            child->hidden_layers[h]->weight.slice(1, 0, split) = h1_weights.slice(1, 0, split);
            child->hidden_layers[h]->weight.slice(1, split, h1_weights.size(1)) =
                    h2_weights.slice(1, split, h2_weights.size(1));
        }

        const auto o1_weights = parent1->output->weight.clone();
        const auto o2_weights = parent2->output->weight.clone();
        int split = o1_weights.size(1) / 2;
        child->output->weight.slice(1, 0, split) = o2_weights.slice(1, 0, split);
        child->output->weight.slice(1, split, o1_weights.size(1)) =
                o2_weights.slice(1, split, o2_weights.size(1));
    }
}
