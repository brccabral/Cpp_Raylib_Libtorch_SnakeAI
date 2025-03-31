#include <mlgames/Linear_QNet.h>

LinearNNImpl::LinearNNImpl(size_t input_size_, size_t hidden_size_, size_t output_size_)
{
    input_size = input_size_;
    hidden_size = hidden_size_;
    output_size = output_size_;

    linear1 = register_module("linear1", torch::nn::Linear(input_size_, hidden_size_));
    linear1->to(torch::kDouble);
    linear2 = register_module("linear2", torch::nn::Linear(hidden_size_, output_size_));
    linear2->to(torch::kDouble);
}

torch::Tensor LinearNNImpl::forward(const torch::Tensor &x)
{
    torch::Tensor output = relu(linear1->forward(x));
    output = linear2->forward(output);
    return output;
}

void save_model(const LinearNN *model, const char *filename)
{
    torch::save(*model, filename);
}

void load_model(LinearNN *model, const char *filename)
{
    torch::load(*model, filename);
}
