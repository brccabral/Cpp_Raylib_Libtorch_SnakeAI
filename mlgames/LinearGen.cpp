#include <mlgames/LinearGen.h>

LinearGenImpl::LinearGenImpl(
        size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_)
{
    input_size = input_size_;
    hidden_sizes = hidden_sizes_;
    output_size = output_size_;

    size_t previous = input_size_;
    size_t current{};
    const char *prefix = "linear";

    for (size_t h = 0; h < hidden_sizes_.size(); ++h)
    {
        current = hidden_sizes_[h];
        hidden_layers.push_back(register_module(
                std::string(prefix) + std::to_string(h), torch::nn::Linear(previous, current)));
        hidden_layers[h]->to(torch::kFloat);
        previous = current;
    }

    output = register_module("output", torch::nn::Linear(previous, output_size_));
    output->to(torch::kFloat);
}

torch::Tensor LinearGenImpl::forward(const torch::Tensor &x)
{
    torch::Tensor result = x;
    for (size_t h = 0; h < hidden_sizes.size(); ++h)
    {
        result = relu(hidden_layers[h]->forward(result));
    }
    result = torch::softmax(output->forward(result), 1);
    return result;
}

LinearGen LinearGenImpl::clone() const
{
    auto clone = LinearGen(this->input_size, this->output_size, this->hidden_sizes);
    for (int p = 0; p < parameters().size(); ++p)
    {
        clone->parameters()[p].data() = parameters()[p].data().clone();
    }
    return clone;
}
