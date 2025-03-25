#include "QTrainer.h"

QTrainer::QTrainer(
        Linear_QNet *model_, const float lr_, const float gamma_, const c10::DeviceType device_)
{
    model = model_;
    lr = lr_;
    gamma = gamma_;

    device = device_;
    optimizer = new torch::optim::Adam((*model)->parameters(), lr);
}

QTrainer::~QTrainer()
{
    delete optimizer;
}
