#pragma once

#include "MultiLayer.h"


class LinearNNImpl : public MultiLayer
{
public:

    LinearNNImpl(size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_);
    torch::Tensor forward(const torch::Tensor &x) override;
};
// TORCH_MODULE = wrapper for `std::shared_ptr<LinearNNImpl>`
TORCH_MODULE(LinearNN);
