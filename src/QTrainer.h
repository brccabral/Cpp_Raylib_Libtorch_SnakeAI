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
    template<int N_states, int N_actions>
    void train_step(
            const std::vector<std::array<int, N_states>> &old_states_,
            const std::vector<std::array<int, N_actions>> &actions_,
            const std::vector<int> &rewards_,
            const std::vector<std::array<int, N_states>> &new_states_,
            const std::vector<bool> &dones_);
};

template<int N_states, int N_actions>
void QTrainer::train_step(
        const std::vector<std::array<int, N_states>> &old_states_,
        const std::vector<std::array<int, N_actions>> &actions_, const std::vector<int> &rewards_,
        const std::vector<std::array<int, N_states>> &new_states_, const std::vector<bool> &dones_)
{
    torch::Tensor old_states =
            torch::from_blob((void *) old_states_.data(), {(long) old_states_.size(), N_states});
    old_states = old_states.to(device);
    torch::Tensor actions =
            torch::from_blob((void *) actions_.data(), {(long) actions_.size(), N_actions});
    actions = actions.to(device);
    torch::Tensor rewards = torch::tensor(rewards_);
    rewards = rewards.to(device);
    torch::Tensor new_states =
            torch::from_blob((void *) new_states_.data(), {(long) new_states_.size(), N_states});
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
            q_new = rewards[index] + gamma * torch::max((*model)->forward(new_states[index]));
        }
        target[index][torch::argmax(actions[index]).item().toInt()] = q_new;
    }
    optimizer->zero_grad();
    torch::Tensor loss = criterion(target, pred_action);
    loss.backward();
    optimizer->step();
}
