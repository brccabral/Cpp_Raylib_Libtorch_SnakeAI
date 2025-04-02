#pragma once

#include <torch/torch.h>


class MultiLayer : public torch::nn::Module
{
public:

    MultiLayer();
    ~MultiLayer() override;

    virtual torch::Tensor forward(const torch::Tensor &x) = 0;

    size_t input_size{};
    std::vector<size_t> hidden_sizes{};
    size_t output_size{};
    std::vector<torch::nn::Linear> hidden_layers{};
    torch::nn::Linear output{nullptr};
};

void save_model(std::shared_ptr<MultiLayer> model, const char *filename = "models/MultiLayer.pt");
void load_model(std::shared_ptr<MultiLayer> model, const char *filename = "models/MultiLayer.pt");
