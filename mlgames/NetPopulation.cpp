#include <random>
#include <mlgames/NetPopulation.h>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> mutation_dist(-0.1, 0.1);

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
            mutate(clone);
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
        mutate(child);
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
    const int split = std::uniform_int_distribution<int>(0, h1_weights.cols() - 1)(gen);
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

void mutate_matrix(MLMatrix &input, double mutation_rate)
{
    MLMatrix prob = MLMatrix::Random(input.rows(), input.cols()).cwiseAbs();
    MLMatrix mask = (prob.array() < mutation_rate).cast<double>();

    Eigen::MatrixXi condition = Eigen::MatrixXi::Random(input.rows(), input.cols());
    condition = condition.cwiseAbs();
    condition = (condition * 3).array().unaryExpr([](const auto &val) { return val % 3; });

    MLMatrix mask_0 = (condition.array() == 0).cast<double>();
    MLMatrix mask_1 = (condition.array() == 1).cast<double>();
    MLMatrix mask_2 = (condition.array() == 2).cast<double>();

    MLMatrix mutation_values = MLMatrix::Random(input.rows(), input.cols());
    input = input.array() * (1 - mask_0.array()) + mutation_values.array() * mask_0.array();
    input = (input.array() - input.array() * mask_1.array()) +
            (input.array() * mutation_values.array() * mask_1.array());
    input = (input.array() + mask_2.array() * mutation_values.array()).min(1.0).max(-1.0);
}

void mutate_vector(MLVector &input, double mutation_rate)
{
    MLMatrix prob = MLMatrix::Random(input.rows(), input.cols()).cwiseAbs();
    MLMatrix mask = (prob.array() < mutation_rate).cast<double>();

    Eigen::MatrixXi condition = Eigen::MatrixXi::Random(input.rows(), input.cols());
    condition = condition.cwiseAbs();
    condition = condition.array().unaryExpr([](const auto &val) { return val % 3; });

    MLMatrix mask_0 = (condition.array() == 0).cast<double>();
    MLMatrix mask_1 = (condition.array() == 1).cast<double>();
    MLMatrix mask_2 = (condition.array() == 2).cast<double>();

    MLMatrix mutation_values = MLMatrix::Random(input.rows(), input.cols());
    input = input.array() * (1 - mask_0.array()) + mutation_values.array() * mask_0.array();
    input = (input.array() - input.array() * mask_1.array()) +
            (input.array() * mutation_values.array() * mask_1.array());
    input = (input.array() + mask_2.array() * mutation_values.array()).min(1.0).max(-1.0);
}

void NetPopulation::mutate(NetGen &net) const
{
    for (size_t h = 0; h < net.ml_count_hidden; ++h)
    {
        mutate_matrix(net.ml_hidden_w[h], mutation_rate);
        mutate_vector(net.ml_hidden_b[h], mutation_rate);
    }
    mutate_matrix(net.ml_output_w, mutation_rate);
    mutate_vector(net.ml_output_b, mutation_rate);
}
