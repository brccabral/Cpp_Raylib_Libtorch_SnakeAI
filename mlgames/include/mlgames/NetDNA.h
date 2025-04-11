#pragma once

class Neuron
{
public:

    Neuron();
    explicit Neuron(int count_weights_);
    Neuron(const Neuron &other);
    Neuron(Neuron &&other) noexcept;
    Neuron &operator=(const Neuron &other);
    ~Neuron();

    int count_weights{};
    float *weights = nullptr;
    float error{};
    float forward{};

private:

    void copy(const Neuron &other);
};


class Layer
{
public:

    Layer();
    Layer(int count_neurons_, int count_weights_);
    Layer(const Layer &other);
    Layer(Layer &&other) noexcept;
    Layer &operator=(const Layer &other);
    ~Layer();

    int count_neurons{};
    Neuron *neurons = nullptr;

private:

    void copy(const Layer &other);
};

class NetDNA
{
public:

    NetDNA(int num_hidden, int num_input_neurons, int num_hidden_neurons, int num_output_neurons);
    NetDNA(const NetDNA &other);
    NetDNA(NetDNA &&other) noexcept;
    NetDNA &operator=(const NetDNA &other);
    ~NetDNA();

    void forward(const float *inputs_, float *result);
    NetDNA clone() const;

    Layer input{};
    Layer output{};
    int count_hidden{};
    Layer *hidden = nullptr;

private:

    void copy(const NetDNA &other);
};
