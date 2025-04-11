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
    // result = torch::softmax(output->forward(result), 1);
    result = relu(output->forward(result));
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

void LinearGenImpl::mutate(double mutation_rate) const
{
    for (auto &parameter: parameters())
    {
        auto data = parameter.data();
        // mask = random select what values to change, mutation_rate reduces with each generation
        auto mask = torch::rand_like(data).abs() < mutation_rate;
        auto condition = torch::randint_like(data, 0, 2);
        // mask_0 = replaces values
        auto mask_0 = (condition == 0) & mask;
        // mask_1 = multiplies current value
        auto mask_1 = (condition == 1) & mask;
        // mask_2 = sum values
        auto mask_2 = (condition == 2) & mask;
        // new values (replace, multiply or sum)
        auto mutation_values = torch::rand_like(data) * 2 - 1;

        data.masked_scatter_(mask_0, mutation_values);
        data = (data - data * mask_1) + ((data * mutation_values) * mask_1);
        data = (data + mask_2 * mutation_values).clamp(-1, 1);
    }
}
