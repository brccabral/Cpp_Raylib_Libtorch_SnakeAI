#pragma once
#include "Linear_QNet.h"


class QTrainer
{
    Linear_QNet *model;
    double gamma;

    torch::optim::Optimizer *optimizer;
    torch::nn::MSELoss criterion{};
    c10::DeviceType device;

public:

    QTrainer(
            Linear_QNet *model_, torch::optim::Optimizer *optimizer_, double gamma_,
            c10::DeviceType device_);
    void train_step(
            size_t count_samples, const std::vector<double> &old_states_,
            const std::vector<int> &actions_, const std::vector<int> &rewards_,
            const std::vector<double> &new_states_, const std::vector<bool> &dones_);
};
