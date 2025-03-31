#pragma once

#include <string>
#include <torch/torch.h>


class Linear_QNetImpl : public torch::nn::Module
{
public:

    Linear_QNetImpl(size_t input_size_, size_t hidden_size_, size_t output_size_);
    torch::Tensor forward(const torch::Tensor &x);

    size_t input_size;
    size_t hidden_size;
    size_t output_size;

private:

    torch::nn::Linear linear1{nullptr};
    torch::nn::Linear linear2{nullptr};
};
// TORCH_MODULE = wrapper for `std::shared_ptr<Linear_QNetImpl>`
TORCH_MODULE(Linear_QNet);

void save_model(const Linear_QNet *model, const char *filename = "models/model.pt");
void load_model(Linear_QNet *model, const char *filename = "models/model.pt");
