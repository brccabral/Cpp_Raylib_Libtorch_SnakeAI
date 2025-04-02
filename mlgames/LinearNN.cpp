#include <mlgames/LinearNN.h>

LinearNNImpl::LinearNNImpl(
        size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_)
    : MultiLayer(input_size_, output_size_, hidden_sizes_)
{}

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
