#include <mlgames/MultiLayer.h>

MultiLayer::MultiLayer(
        size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_)
{
    input_size = input_size_;
    hidden_sizes = hidden_sizes_;
    output_size = output_size_;

    size_t previous = input_size_;
    size_t current{};
    const char *prefix = "linear";

    hidden_layers.reserve(hidden_sizes.size());

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
};

MultiLayer::~MultiLayer() = default;
