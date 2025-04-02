#include <mlgames/LinearGen.h>

LinearGenImpl::LinearGenImpl(
        size_t input_size_, size_t output_size_, const std::vector<size_t> &hidden_sizes_)
    : MultiLayer(input_size_, output_size_, hidden_sizes_)
{}

torch::Tensor LinearGenImpl::forward(const torch::Tensor &x)
{
    torch::Tensor result = x;
    for (size_t h = 0; h < hidden_sizes.size(); ++h)
    {
        result = relu(hidden_layers[h]->forward(result));
    }
    result = torch::softmax(output->forward(result), 1);
    return result;
}

LinearGen LinearGenImpl::clone() const
{
    auto clone = LinearGen(this->input_size, this->output_size, this->hidden_sizes);
    for (int p = 0; p < parameters().size(); ++p)
    {
        clone->parameters()[p].data() = parameters()[p].data().clone();
    }
    return clone;
}
