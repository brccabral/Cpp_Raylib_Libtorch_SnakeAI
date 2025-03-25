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

    QTrainer(Linear_QNet *model_, float lr_, float gamma_, c10::DeviceType device_);
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
    std::vector<int> flatten_old_states;
    flatten_old_states.reserve(old_states_.size() * N_states);
    std::vector<int> flatten_actions;
    flatten_actions.reserve(actions_.size() * N_actions);
    std::vector<int> flatten_rewards;
    flatten_rewards.reserve(rewards_.size());
    std::vector<int> flatten_new_states;
    flatten_new_states.reserve(new_states_.size() * N_states);

    // all inputs have same size()
    for (size_t i = 0; i < old_states_.size(); ++i)
    {
        flatten_old_states.insert(
                flatten_old_states.end(), old_states_[i].begin(), old_states_[i].end());
        flatten_actions.insert(flatten_actions.end(), actions_[i].begin(), actions_[i].end());
        flatten_rewards.insert(flatten_rewards.end(), 1, rewards_[i]);
        flatten_new_states.insert(
                flatten_new_states.end(), new_states_[i].begin(), new_states_[i].end());
    }


    torch::Tensor old_states = torch::tensor(flatten_old_states, torch::kFloat)
                                       .reshape({(long) old_states_.size(), N_states});
    old_states = old_states.to(device);
    torch::Tensor actions = torch::tensor(flatten_actions, torch::kFloat)
                                    .reshape({(long) actions_.size(), N_actions});
    actions = actions.to(device);
    torch::Tensor rewards =
            torch::tensor(flatten_rewards, torch::kFloat).reshape({(long) rewards_.size(), 1});
    rewards = rewards.to(device);
    torch::Tensor new_states = torch::tensor(flatten_new_states, torch::kFloat)
                                       .reshape({(long) new_states_.size(), N_states});
    new_states = new_states.to(device);

    // 1: predict Q values with current state
    torch::Tensor pred_action = (*model)->forward(old_states);

    // 2: Q_new = r + y * max(next_predicted_Q_value) -> only do this if not done
    torch::Tensor target = pred_action.clone();
    for (size_t index = 0; index < dones_.size(); ++index)
    {
        auto q_new = rewards[index].item().toFloat();
        if (!dones_[index])
        {
            q_new = q_new +
                    gamma * torch::max((*model)->forward(new_states[index])).item().toFloat();
        }
        long max_index = torch::argmax(actions[index]).item().toLong();
        target[index][max_index] = q_new;
    }
    optimizer->zero_grad();
    torch::Tensor loss = criterion(target, pred_action);
    loss.backward();
    optimizer->step();
}
