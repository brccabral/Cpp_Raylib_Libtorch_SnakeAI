#include <mlgames/LinearNN.h>

LinearNNImpl::LinearNNImpl(
        size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_)
{
    input_size = input_size_;
    hidden_sizes = hidden_sizes_;
    output_size = output_size_;

    hidden_layers.reserve(hidden_sizes.size());

    size_t previous = input_size_;
    size_t current{};
    // NOLINTNEXTLINE
    const char *prefix = "linear";

    for (size_t h = 0; h < hidden_sizes_.size(); ++h)
    {
        current = hidden_sizes_[h];
        hidden_layers[h] = register_module(
                std::string(prefix) + std::to_string(h), torch::nn::Linear(previous, current));
        hidden_layers[h]->to(torch::kDouble);
        previous = current;
    }

    output = register_module("output", torch::nn::Linear(previous, output_size_));
    output->to(torch::kDouble);
}

torch::Tensor LinearNNImpl::forward(const torch::Tensor &x)
{
    torch::Tensor result = x;
    for (size_t h = 0; h < hidden_sizes.size(); ++h)
    {
        result = relu(hidden_layers[h]->forward(result));
    }
    result = output->forward(result);
    return result;
}

void save_model(const LinearNN *model, const char *filename)
{
    torch::save(*model, filename);
}

void load_model(LinearNN *model, const char *filename)
{
    torch::load(*model, filename);
}
