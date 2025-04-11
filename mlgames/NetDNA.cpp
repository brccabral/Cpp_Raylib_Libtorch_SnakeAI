#include <random>
#include <mlgames/NetDNA.h>

#define BIAS 1

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> rand_weight(-1.0f, 1.0f);

Neuron::Neuron() = default;

Neuron::Neuron(int count_weights_)
{
    count_weights = count_weights_;
    error = 0;
    forward = 1;
    if (count_weights > 0)
    {
        weights = new float[count_weights];
        for (int i = 0; i < count_weights; i++)
        {
            weights[i] = rand_weight(gen);
        }
    }
}

void Neuron::copy(const Neuron &other)
{
    count_weights = other.count_weights;
    error = other.error;
    forward = other.forward;
    if (other.weights != nullptr)
    {
        weights = new float[count_weights];
        for (int i = 0; i < count_weights; i++)
        {
            weights[i] = other.weights[i];
        }
    }
}

Neuron::Neuron(const Neuron &other)
{
    copy(other);
}

Neuron::Neuron(Neuron &&other) noexcept
{
    count_weights = other.count_weights;
    error = other.error;
    forward = other.forward;
    weights = other.weights;
    other.weights = nullptr;
}

Neuron &Neuron::operator=(const Neuron &other)
{
    if (this != &other)
    {
        delete[] weights;
        copy(other);
    }
    return *this;
}

Neuron::~Neuron()
{
    delete[] weights;
}

Layer::Layer() = default;

Layer::Layer(int count_neurons_, int count_weights_)
{
    count_neurons = count_neurons_;
    neurons = new Neuron[count_neurons];
    for (int i = 0; i < count_neurons; i++)
    {
        neurons[i] = Neuron(count_weights_);
    }
}

void Layer::copy(const Layer &other)
{
    count_neurons = other.count_neurons;
    neurons = new Neuron[count_neurons];
    if (other.neurons != nullptr)
    {
        for (int i = 0; i < count_neurons; i++)
        {
            neurons[i] = other.neurons[i];
        }
    }
}

Layer::Layer(const Layer &other)
{
    copy(other);
}

Layer::Layer(Layer &&other) noexcept
{
    count_neurons = other.count_neurons;
    neurons = other.neurons;
    other.neurons = nullptr;
}

Layer &Layer::operator=(const Layer &other)
{
    if (this != &other)
    {
        delete[] neurons;
        copy(other);
    }
    return *this;
}

Layer::~Layer()
{
    delete[] neurons;
}


NetDNA::NetDNA(
        int num_hidden, int num_input_neurons, int num_hidden_neurons, int num_output_neurons)
{
    count_hidden = num_hidden;

    num_input_neurons = num_input_neurons + BIAS;
    num_hidden_neurons = num_hidden_neurons + BIAS;

    input = Layer(num_input_neurons, 0);
    hidden = new Layer[num_hidden];
    for (auto i = 0; i < count_hidden; ++i)
    {
        if (i == 0)
        {
            hidden[i] = Layer(num_hidden_neurons, num_input_neurons);
        }
        else
        {
            hidden[i] = Layer(num_hidden_neurons, num_hidden_neurons);
        }
    }
    output = Layer(num_output_neurons, num_hidden_neurons);
}

void NetDNA::copy(const NetDNA &other)
{
    input = other.input;
    count_hidden = other.count_hidden;
    hidden = new Layer[count_hidden];
    for (auto i = 0; i < count_hidden; ++i)
    {
        hidden[i] = other.hidden[i];
    }
    output = other.output;
}

NetDNA::NetDNA(const NetDNA &other)
{
    copy(other);
}

NetDNA::NetDNA(NetDNA &&other) noexcept
{
    input = other.input;
    count_hidden = other.count_hidden;
    if (other.hidden != nullptr)
    {
        hidden = other.hidden;
    }
    output = other.output;

    other.hidden = nullptr;
}

NetDNA &NetDNA::operator=(const NetDNA &other)
{
    if (this != &other)
    {
        delete[] hidden;
        copy(other);
    }
    return *this;
}

NetDNA::~NetDNA()
{
    delete[] hidden;
}

float relu(float X)
{
    if (X < 0)
    {
        return 0;
    }
    if (X < 10000)
    {
        return X;
    }
    return 10000;
}

void NetDNA::forward(const float *inputs_, float *result)
{
    // set inputs
    for (auto i = 0; i < input.count_neurons - BIAS; ++i)
    {
        input.neurons[i].forward = inputs_[i];
    }

    float sum{};

    // first hidden * input
    for (int i = 0; i < hidden[0].count_neurons - BIAS; i++)
    {
        sum = 0;
        for (int j = 0; j < input.count_neurons; j++)
        {
            sum = sum + input.neurons[j].forward * hidden[0].neurons[i].weights[j];
        }
        hidden[0].neurons[i].forward = relu(sum);
    }

    // hidden[k] * hidden[k-1]
    for (int k = 1; k < count_hidden; k++)
    {
        for (int i = 0; i < hidden[k].count_neurons - BIAS; i++)
        {
            sum = 0;
            for (int j = 0; j < hidden[k - 1].count_neurons; j++)
            {
                sum = sum + hidden[k - 1].neurons[j].forward * hidden[k].neurons[i].weights[j];
            }
            hidden[k].neurons[i].forward = relu(sum);
        }
    }

    // output * hidden[H]
    for (int i = 0; i < output.count_neurons; i++)
    {
        sum = 0;
        for (int j = 0; j < hidden[count_hidden - 1].count_neurons; j++)
        {
            sum = sum + hidden[count_hidden - 1].neurons[j].forward * output.neurons[i].weights[j];
        }
        output.neurons[i].forward = relu(sum);
        result[i] = output.neurons[i].forward;
    }
}

void clone_Neuron(const Neuron &src, Neuron &dst)
{
    dst.count_weights = src.count_weights;
    dst.error = src.error;
    dst.forward = src.forward;
    for (auto i = 0; i < src.count_weights; ++i)
    {
        dst.weights[i] = src.weights[i];
    }
}

void clone_Layer(const Layer &src, Layer &dst)
{
    dst.count_neurons = src.count_neurons;
    for (int i = 0; i < src.count_neurons; i++)
    {
        clone_Neuron(src.neurons[i], dst.neurons[i]);
    }
}

NetDNA NetDNA::clone() const
{
    NetDNA result =
            NetDNA(count_hidden, input.count_neurons - BIAS, hidden[0].count_neurons - BIAS,
                   output.count_neurons);

    clone_Layer(input, result.input);
    for (auto i = 0; i < count_hidden; ++i)
    {
        clone_Layer(hidden[i], result.hidden[i]);
    }
    clone_Layer(output, result.output);

    return result;
}
