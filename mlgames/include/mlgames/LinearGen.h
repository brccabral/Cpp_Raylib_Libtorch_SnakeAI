#pragma once

#include "MultiLayer.h"

class LinearGen;
class LinearGenImpl : public MultiLayer
{
public:

    LinearGenImpl(
            size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_);
    torch::Tensor forward(const torch::Tensor &x) override;
    LinearGen clone();
};
TORCH_MODULE(LinearGen);
