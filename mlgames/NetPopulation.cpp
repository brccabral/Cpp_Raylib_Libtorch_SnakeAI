#include <random>
#include <mlgames/NetPopulation.h>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<double> dist(-1.0, 1.0);

NetPopulation::NetPopulation(
        size_t population_size_, double mutation_rate_, double mutation_rate_lower_,
        const NetGen &net)
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

void NetPopulation::apply_mutations(std::array<size_t, 2> best_indexes)
{
    std::vector<NetGen> new_members;
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

void NetPopulation::crossover(const NetGen &parent1, const NetGen &parent2, NetGen &child)
{
    for (size_t h = 0; h < parent1.hidden_sizes.size(); ++h)
    {
        const auto h1_weights = parent1.ml_hidden_w[h];
        const auto h2_weights = parent2.ml_hidden_w[h];
        const int split = std::uniform_int_distribution<int>(0, h1_weights.rows() - 1)(gen);
        child.ml_hidden_w[h].block(0, 0, split, h1_weights.cols()) =
                h1_weights.block(0, 0, split, h1_weights.cols());
        child.ml_hidden_w[h].block(split, 0, h2_weights.rows() - split, h2_weights.cols()) =
                h2_weights.block(split, 0, h2_weights.rows() - split, h2_weights.cols());
        const auto h1_bias = parent1.ml_hidden_b[h];
        const auto h2_bias = parent2.ml_hidden_b[h];
        child.ml_hidden_b[h].segment(0, split) = parent1.ml_hidden_b[h].segment(0, split);
        child.ml_hidden_b[h].segment(split, h2_bias.size() - split) =
                parent2.ml_hidden_b[h].segment(split, h2_bias.size() - split);
    }
    const auto h1_weights = parent1.ml_output_w;
    const auto h2_weights = parent2.ml_output_w;
    const int split = std::uniform_int_distribution<int>(0, h1_weights.rows() - 1)(gen);
    child.ml_output_w.block(0, 0, split, h1_weights.cols()) =
            h1_weights.block(0, 0, split, h1_weights.cols());
    child.ml_output_w.block(split, 0, h2_weights.rows() - split, h2_weights.cols()) =
            h2_weights.block(split, 0, h2_weights.rows() - split, h2_weights.cols());
    const auto h1_bias = parent1.ml_output_b;
    const auto h2_bias = parent2.ml_output_b;
    child.ml_output_b.segment(0, split) = parent1.ml_output_b.segment(0, split);
    child.ml_output_b.segment(split, h2_bias.size() - split) =
            parent2.ml_output_b.segment(split, h2_bias.size() - split);
}
