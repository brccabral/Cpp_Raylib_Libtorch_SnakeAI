#include <mlgames/Linear_QNet.h>

Linear_QNetImpl::Linear_QNetImpl(int input_size, int hidden_size, int output_size)
{
    linear1 = register_module("linear1", torch::nn::Linear(input_size, hidden_size));
    linear2 = register_module("linear2", torch::nn::Linear(hidden_size, output_size));
}

torch::Tensor Linear_QNetImpl::forward(const torch::Tensor &x)
{
    torch::Tensor output = relu(linear1->forward(x));
    output = linear2->forward(output);
    return output;
}

void save_model(const Linear_QNet *model, const char *filename)
{
    torch::save(*model, filename);
}

void load_model(Linear_QNet *model, const char *filename)
{
    torch::load(*model, filename);
}
