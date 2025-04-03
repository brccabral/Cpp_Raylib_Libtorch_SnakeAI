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
        mutate(clone);
        members.push_back(clone);
    }
}

void GenPopulation::apply_mutations(std::array<size_t, 2> best_indexes)
{
    std::vector<LinearGen> new_members;
    // leave the best untouched
    new_members.push_back(members[best_indexes[0]]);

    for (size_t i = 1; i < population_size; i++)
    {
        auto child = members[best_indexes[0]]->clone();
        crossover(members[best_indexes[0]], members[best_indexes[1]], child);
        mutate(child);
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

void GenPopulation::mutate(const LinearGen &net) const
{
    for (auto &parameter: net->parameters())
    {
        auto data = parameter.data();
        // mask = random select what values to change, mutation_rate reduces with each generation
        auto mask = torch::rand_like(data) < mutation_rate;
        auto condition = torch::randint_like(data, 0, 2);
        // mask_0 = replaces values
        auto mask_0 = (condition == 0) & mask;
        // mask_1 = multiplies current value
        auto mask_1 = (condition == 1) & mask;
        // mask_2 = sum values
        auto mask_2 = (condition == 2) & mask;
        // new values (replace, multiply or sum)
        auto mutation_values = torch::rand_like(data) * 2 - 1;

        data.masked_scatter_(mask_0, mutation_values);
        data = (data - data * mask_1) + ((data * mutation_values) * mask_1);
        data = (data + mask_2 * mutation_values).clamp(-1, 1);
    }
}
