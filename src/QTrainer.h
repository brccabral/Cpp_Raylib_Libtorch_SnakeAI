#pragma once
#include "Linear_QNet.h"


template<int N_states, int N_actions>
class QTrainer
{
    Linear_QNet *model;
    float gamma;

    torch::optim::Optimizer *optimizer;
    torch::nn::MSELoss criterion{};
    c10::DeviceType device;

public:

    QTrainer(
            Linear_QNet *model_, torch::optim::Optimizer *optimizer_, float gamma_,
            c10::DeviceType device_);
    void train_step(
            const std::vector<std::array<int, N_states>> &old_states_,
            const std::vector<std::array<int, N_actions>> &actions_,
            const std::vector<int> &rewards_,
            const std::vector<std::array<int, N_states>> &new_states_,
            const std::vector<bool> &dones_);
};

template<int N_states, int N_actions>
QTrainer<N_states, N_actions>::QTrainer(
        Linear_QNet *model_, torch::optim::Optimizer *optimizer_, const float gamma_,
        const c10::DeviceType device_)
{
    model = model_;
    gamma = gamma_;

    device = device_;
    optimizer = optimizer_;
}


template<int N_states, int N_actions>
void QTrainer<N_states, N_actions>::train_step(
        const std::vector<std::array<int, N_states>> &old_states_,
        const std::vector<std::array<int, N_actions>> &actions_, const std::vector<int> &rewards_,
        const std::vector<std::array<int, N_states>> &new_states_, const std::vector<bool> &dones_)
{
    std::vector<int> flatten_old_states;
    flatten_old_states.reserve(old_states_.size() * N_states);
    std::vector<int> flatten_actions;
    flatten_actions.reserve(actions_.size() * N_actions);
    std::vector<int> flatten_new_states;
    flatten_new_states.reserve(new_states_.size() * N_states);

    // all inputs have same size()
    for (size_t i = 0; i < old_states_.size(); ++i)
    {
        flatten_old_states.insert(
                flatten_old_states.end(), old_states_[i].begin(), old_states_[i].end());
        flatten_actions.insert(flatten_actions.end(), actions_[i].begin(), actions_[i].end());
        flatten_new_states.insert(
                flatten_new_states.end(), new_states_[i].begin(), new_states_[i].end());
    }


    torch::Tensor old_states = torch::tensor(flatten_old_states, torch::kInt)
                                       .reshape({(long) old_states_.size(), N_states})
                                       .to(device, torch::kFloat);
    torch::Tensor actions = torch::tensor(flatten_actions, torch::kInt)
                                    .reshape({(long) actions_.size(), N_actions})
                                    .to(device, torch::kFloat);
    torch::Tensor new_states = torch::tensor(flatten_new_states, torch::kInt)
                                       .reshape({(long) new_states_.size(), N_states})
                                       .to(device, torch::kFloat);

    // 1: predict Q values with current state
    (*model)->train();
    torch::Tensor pred_action = (*model)->forward(old_states);

    // 2: Q_new = r + y * max(next_predicted_Q_value) -> only do this if not done
    (*model)->eval();
    torch::Tensor target = pred_action.detach().clone();
    {
        torch::NoGradGuard no_grad;
        for (size_t index = 0; index < dones_.size(); ++index)
        {
            auto q_new = rewards_[index];
            const long max_index = torch::argmax(actions[index]).item().toLong();
            if (!dones_[index])
            {
                q_new = q_new +
                        gamma * (*model)->forward(new_states[index])[max_index].item().toFloat();
            }
            target[index][max_index] = q_new;
        }
    }

    (*model)->train();
    optimizer->zero_grad();
    const torch::Tensor loss = criterion(pred_action, target);
    loss.backward();
    optimizer->step();
}
