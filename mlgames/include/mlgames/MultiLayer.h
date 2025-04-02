#pragma once

#include <torch/torch.h>


class MultiLayer : public torch::nn::Module
{
public:

    MultiLayer(size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_);
    ~MultiLayer() override;

    virtual torch::Tensor forward(const torch::Tensor &x) = 0;

    size_t input_size{};
    std::vector<size_t> hidden_sizes{};
    size_t output_size{};
    std::vector<torch::nn::Linear> hidden_layers{};
    torch::nn::Linear output{nullptr};
};

template<typename T>
void save_model(const torch::nn::ModuleHolder<T> &model, const char *filename = "models/model.pt")
{
    torch::save(model, filename);
}

template<typename T>
void load_model(torch::nn::ModuleHolder<T> model, const char *filename = "models/model.pt")
{
    torch::load(model, filename);
}
