#include "QTrainer.h"

QTrainer::QTrainer(Linear_QNet *model_, const float lr_, const float gamma_)
{
    model = model_;
    lr = lr_;
    gamma = gamma_;

    optimizer = new torch::optim::Adam((*model)->parameters(), lr);

    device = torch::kCPU;
    if (torch::cuda::is_available())
    {
        printf("Using CUDA\n");
        device = torch::kCUDA;
    }
    else
    {
        printf("Using CPU\n");
        device = torch::kCPU;
    }
}

QTrainer::~QTrainer()
{
    delete optimizer;
}
