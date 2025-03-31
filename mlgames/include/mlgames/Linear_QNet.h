#pragma once

#include <torch/torch.h>


class LinearNNImpl : public torch::nn::Module
{
public:

    LinearNNImpl(size_t input_size_, size_t hidden_size_, size_t output_size_);
    torch::Tensor forward(const torch::Tensor &x);

    size_t input_size;
    size_t hidden_size;
    size_t output_size;

private:

    torch::nn::Linear linear1{nullptr};
    torch::nn::Linear linear2{nullptr};
};
// TORCH_MODULE = wrapper for `std::shared_ptr<LinearNNImpl>`
TORCH_MODULE(LinearNN);

void save_model(const LinearNN *model, const char *filename = "models/model.pt");
void load_model(LinearNN *model, const char *filename = "models/model.pt");
