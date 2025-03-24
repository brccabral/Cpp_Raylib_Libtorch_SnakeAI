#pragma once
#include "Linear_QNet.h"

class QTrainer
{
    Linear_QNet *model;
    float lr;
    float gamma;

    torch::optim::Adam *optimizer;
    torch::nn::MSELoss criterion;
    c10::DeviceType device;

public:

    QTrainer(Linear_QNet *model_, float lr_, float gamma_);
    ~QTrainer();
    template<int Nstates>
    void train_step(
            std::vector<std::array<int, Nstates>> old_states_, std::vector<int> actions_,
            std::vector<int> rewards_, std::vector<std::array<int, Nstates>> new_states_,
            std::vector<bool> dones_);
};

template<int Nstates>
void QTrainer::train_step(
        std::vector<std::array<int, Nstates>> old_states_, std::vector<int> actions_,
        std::vector<int> rewards_, std::vector<std::array<int, Nstates>> new_states_,
        std::vector<bool> dones_)
{
    torch::Tensor old_states =
            torch::from_blob(old_states_.data(), {(long) old_states_.size(), Nstates});
    old_states = old_states.to(device);
    torch::Tensor actions = torch::tensor(actions_, torch::kInt32);
    actions = actions.to(device);
    torch::Tensor rewards = torch::tensor(rewards_);
    rewards = rewards.to(device);
    torch::Tensor new_states =
            torch::from_blob(new_states_.data(), {(long) new_states_.size(), Nstates});
    new_states = new_states.to(device);

    // 1: predict Q values with current state
    torch::Tensor pred_action = (*model)->forward(old_states);

    // 2: Q_new = r + y * max(next_predicted_Q_value) -> only do this if not done
    torch::Tensor target = pred_action.clone();
    for (size_t index = 0; index < dones_.size(); ++index)
    {
        auto q_new = rewards[index];
        if (!dones_[index])
        {
            q_new = rewards[index] + gamma * torch::max((*model)->forward(new_states));
        }
        target[index][torch::argmax(actions).item()] = q_new;
    }
    optimizer->zero_grad();
    torch::Tensor loss = criterion(target, pred_action);
    loss.backward();
    optimizer->step();
}
