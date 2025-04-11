#include <assert.h>
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
            mutate(clone);
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
        mutate(child);
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

void PopulationDNA::mutate(NetDNA &net) const
{
    // find size of DNA weights
    int size_dna = 0;
    for (int h = 0; h < net.count_hidden; h++)
    {
        for (auto n = 0; n < net.hidden[h].count_neurons; ++n)
        {
            size_dna += net.hidden[h].neurons[n].count_weights;
        }
    }
    for (auto n = 0; n < net.output.count_neurons; ++n)
    {
        size_dna += net.output.neurons[n].count_weights;
    }

    // copy weights into DNA
    auto *dna = new float[size_dna];
    int d = 0;
    for (auto h = 0; h < net.count_hidden; ++h)
    {
        for (int n = 0; n < net.hidden[h].count_neurons; n++)
        {
            for (int w = 0; w < net.hidden[h].neurons[n].count_weights; w++)
            {
                dna[d++] = net.hidden[h].neurons[n].weights[n];
            }
        }
    }
    for (int n = 0; n < net.output.count_neurons; n++)
    {
        for (int w = 0; w < net.output.neurons[n].count_weights; w++)
        {
            dna[d++] = net.output.neurons[n].weights[n];
        }
    }
    assert(d == size_dna);

    // mutate randomly
    const auto count_mutations = size_dna * mutation_rate;
    for (auto m = 0; m < count_mutations; m++)
    {
        const int type = std::uniform_int_distribution<int>(0, 2)(gen);
        const int index = std::uniform_int_distribution<int>(0, size_dna)(gen);
        switch (type)
        {
            // replace
            case 0:
            {
                dna[index] = rand_weight(gen);
                break;
            }
            // multiply
            case 1:
            {
                dna[index] *= rand_weight(gen);
                break;
            }
            // sum, clamp
            case 2:
            {
                dna[index] += rand_weight(gen);
                dna[index] = std::min(std::max(dna[index], -1.0f), 1.0f);
                break;
            }
            default:
            {
                (void) fprintf(stderr, "ERROR: Wrong random type.\n");
                break;
            }
        }
    }

    // copy back
    d = 0;
    for (auto h = 0; h < net.count_hidden; ++h)
    {
        for (int n = 0; n < net.hidden[h].count_neurons; n++)
        {
            for (int w = 0; w < net.hidden[h].neurons[n].count_weights; w++)
            {
                net.hidden[h].neurons[n].weights[n] = dna[d++];
            }
        }
    }
    for (int n = 0; n < net.output.count_neurons; n++)
    {
        for (int w = 0; w < net.output.neurons[n].count_weights; w++)
        {
            net.output.neurons[n].weights[n] = dna[d++];
        }
    }
    assert(d == size_dna);

    delete[] dna;
}
