#pragma once

#include <torch/torch.h>


class LinearNNImpl : public torch::nn::Module
{
public:

    LinearNNImpl(size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_);
    torch::Tensor forward(const torch::Tensor &x);

    size_t input_size;
    std::vector<size_t> hidden_sizes;
    size_t output_size;

private:

    std::vector<torch::nn::Linear> hidden_layers;
    torch::nn::Linear output{nullptr};
};
// TORCH_MODULE = wrapper for `std::shared_ptr<LinearNNImpl>`
TORCH_MODULE(LinearNN);

void save_model(const LinearNN *model, const char *filename = "models/LinearNN.pt");
void load_model(LinearNN *model, const char *filename = "models/LinearNN.pt");
